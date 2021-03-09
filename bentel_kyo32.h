#include "esphome.h"

#define SENSOR_CNT 4

class Bentel_Kyo32 : public PollingComponent, public UARTDevice {
 BinarySensor *xsensor1 {nullptr};
 BinarySensor *xsensor2 {nullptr};
 BinarySensor *xsensor3 {nullptr};
 BinarySensor *xsensor4 {nullptr};
 BinarySensor *xsensor5 {nullptr};
 BinarySensor *xsensor6 {nullptr};
 BinarySensor *xsensor7 {nullptr};
 BinarySensor *xsensor8 {nullptr};
 BinarySensor *xsensor9 {nullptr};
 BinarySensor *xsensor10 {nullptr};
 BinarySensor *xsensor11 {nullptr};
 BinarySensor *xsensor12 {nullptr};
 BinarySensor *xsensor13 {nullptr};
 BinarySensor *xsensor14 {nullptr};
 BinarySensor *xsensor15 {nullptr};
 BinarySensor *xsensor16 {nullptr};

 public:
  Bentel_Kyo32(UARTComponent *parent, 
                BinarySensor *sensor1, 
                BinarySensor *sensor2, 
                BinarySensor *sensor3, 
                BinarySensor *sensor4,  
                BinarySensor *sensor5, 
                BinarySensor *sensor6, 
                BinarySensor *sensor7, 
                BinarySensor *sensor8,
                BinarySensor *sensor9, 
                BinarySensor *sensor10, 
                BinarySensor *sensor11, 
                BinarySensor *sensor12,  
                BinarySensor *sensor13, 
                BinarySensor *sensor14, 
                BinarySensor *sensor15, 
                BinarySensor *sensor16) : UARTDevice(parent) , xsensor1(sensor1), xsensor2(sensor2), xsensor3(sensor3), xsensor4(sensor4), xsensor5(sensor5), xsensor6(sensor6), xsensor7(sensor7), xsensor8(sensor8), xsensor9(sensor9), xsensor10(sensor10), xsensor11(sensor11), xsensor12(sensor12), xsensor13(sensor13), xsensor14(sensor14), xsensor15(sensor15), xsensor16(sensor16) {}

  byte cmdGetSensorStatus[6] = {  0xf0, 0x04, 0xf0, 0x0a, 0x00, 0xee};  // f0 04 f0 0a 00 ee
  //byte vorlaufSet[7]    = {  0x07, 0x00, 0x00, 0x00, 0x19, 0x00, 0xd2 };        //antwort: 1+5 bytes
  //byte vorlaufIst[7]    = {  0x07, 0x00, 0x00, 0x00, 0x18, 0x00, 0xd0 };        //antwort: 1+5 bytes
  //byte vorlaufSoll[7]   = {  0x07, 0x00, 0x00, 0x00, 0x39, 0x00, 0x92 };       //antwort: 1+4 bytes -> 40
  //byte ruecklauf[7]     = {  0x07, 0x00, 0x00, 0x00, 0x98, 0x00, 0xc9 };       //antwort: 1+4 bytes 

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

          if (xsensor1 != nullptr && i == 0)
            xsensor1->publish_state(val);

          if (xsensor2 != nullptr && i == 1)
            xsensor2->publish_state(val);

          if (xsensor3 != nullptr && i == 2)
            xsensor3->publish_state(val);

          if (xsensor4 != nullptr && i == 3)
            xsensor4->publish_state(val);

          if (xsensor5 != nullptr && i == 4)
            xsensor5->publish_state(val);

          if (xsensor6 != nullptr && i == 5)
            xsensor6->publish_state(val);

          if (xsensor7 != nullptr && i == 6)
            xsensor7->publish_state(val);

          if (xsensor8 != nullptr && i == 7)
            xsensor8->publish_state(val);
          
          if (xsensor9 != nullptr && i == 0)
            xsensor9->publish_state(val);

          if (xsensor10 != nullptr && i == 1)
            xsensor10->publish_state(val);

          if (xsensor11 != nullptr && i == 2)
            xsensor11->publish_state(val);

          if (xsensor12 != nullptr && i == 3)
            xsensor12->publish_state(val);

          if (xsensor13 != nullptr && i == 4)
            xsensor13->publish_state(val);

          if (xsensor14 != nullptr && i == 5)
            xsensor14->publish_state(val);

          if (xsensor15 != nullptr && i == 6)
            xsensor15->publish_state(val);

          if (xsensor16 != nullptr && i == 7)
            xsensor16->publish_state(val);
        
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