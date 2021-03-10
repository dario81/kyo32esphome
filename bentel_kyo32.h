#include "esphome.h"

#define SENSOR_CNT 4

class Bentel_Kyo32 : public PollingComponent, public UARTDevice {
 public:
  Bentel_Kyo32(UARTComponent *parent) : UARTDevice(parent) {}

  BinarySensor *zona[32];

  byte cmdGetSensorStatus[6] = {  0xf0, 0x04, 0xf0, 0x0a, 0x00, 0xee};  // f0 04 f0 0a 00 ee

  bool getParm(byte *cmd, int lcmd, byte ReadByes[], int &CountBytes) {
    byte RxBuff[255];
    char TraceString[255];

    int index = 0, i = 0;
    
    int val;
    memset(ReadByes, 0, CountBytes);

    write_array(cmd,lcmd);
    delay(100);
    while (available() > 0)
    {
      RxBuff[index++] = read(); // Read a Byte
      delay(3);
    }

    if (index > 0)
    {
        CountBytes = index;
        memcpy(ReadByes, RxBuff, CountBytes);

        return true;
    }
    else
        return false;
  }

  void setup() override {
    this->set_update_interval(5000);
  }

  void loop() override {
  }

  void update() override {
    bool val;
    byte Rx[255];
    int Count = 0, i;

    int StatoZona;
    byte mask = 1;

    val=getParm(cmdGetSensorStatus,sizeof(cmdGetSensorStatus), Rx, Count);

    if (Count == 18)
    {
      // Ciclo ZONE
      for (i = 0; i < 32; i++)
      {
        mask = 00000001;
        StatoZona = -1;

        
          if (i >= 24)
            StatoZona = (Rx[6] >> (i - 24)) & 1;
          else if (i >= 16 && i <= 23)
            StatoZona = (Rx[7] >> (i - 16)) & 1;
          else if (i >= 8 && i <= 15)
            StatoZona = (Rx[8] >> (i - 8)) & 1;
          else if (i <= 7)
            StatoZona = (Rx[9] >> i) & 1;

          //ESP_LOGD("custom", "The value of sensor is: %i", StatoZona);
          if(StatoZona == 1)
          {
            val = true;
          }
          else
          {
            val = false;
          }
          if(zona[i] == nullptr)
          {
            zona[i] = new BinarySensor();
          }
          zona[i]->publish_state(val);

          
        
      }
    }
     

      /*val=getParm(vorlaufIst,sizeof(vorlaufIst));
    if (xsensor2 != nullptr)   xsensor2->publish_state(val);

    val=getParm(vorlaufSet,sizeof(vorlaufSet));
    if (xsensor3 != nullptr)   xsensor3->publish_state(val);

    val=getParm(ruecklauf,sizeof(ruecklauf));
    if (xsensor4 != nullptr)   xsensor4->publish_state(val);
*/
    }
};