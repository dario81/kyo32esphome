#include "esphome.h"

#define SENSOR_CNT 4

class Bentel_Kyo32 : public PollingComponent, public UARTDevice
{
public:
  Bentel_Kyo32(UARTComponent *parent) : UARTDevice(parent) {}

  BinarySensor *zona[32];
  BinarySensor *zona_sabotaggio[32];

  BinarySensor *warn_mancanza_rete = new BinarySensor("Warning Mancanza Rete");
  BinarySensor *warn_scomparsa_bpi = new BinarySensor("Warning Scomparsa BPI");;
  BinarySensor *warn_fusibile = new BinarySensor("Warning Fusibile");;
  BinarySensor *warn_batteria_bassa = new BinarySensor("Warning Batteria Bassa");;
  BinarySensor *warn_guasto_linea_telefonica = new BinarySensor("Warning Guasto Linea Telefonica");;
  BinarySensor *warn_codici_default = new BinarySensor("Warning Codici Default");;
  BinarySensor *warn_wireless = new BinarySensor("Warning Wireless");;

  BinarySensor *allarme_area[8];

  BinarySensor *sabotaggio_zona = new BinarySensor("Sabotaggio di Zona");;
  BinarySensor *sabotaggio_chiave_falsa = new BinarySensor("Sabotaggio Chiave Falsa");;
  BinarySensor *sabotaggio_bpi = new BinarySensor("Sabotaggio BPI");;
  BinarySensor *sabotaggio_sistema = new BinarySensor("Sabotaggio Sistema");;
  BinarySensor *sabotaggio_jam = new BinarySensor("Sabotaggio JAM");;
  BinarySensor *sabotaggio_wireless = new BinarySensor("Sabotaggio Wireless");;

  byte cmdGetSensorStatus[6] = {0xf0, 0x04, 0xf0, 0x0a, 0x00, 0xee}; // f0 04 f0 0a 00 ee

  bool getParm(byte *cmd, int lcmd, byte ReadByes[], int &CountBytes)
  {
    byte RxBuff[255];
    char TraceString[255];

    int index = 0, i = 0;

    int val;
    memset(ReadByes, 0, CountBytes);

    write_array(cmd, lcmd);
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

  void setup() override
  {
    this->set_update_interval(5000);
  }

  void loop() override
  {
  }

  void update() override
  {
    bool val;
    byte Rx[255];
    int Count = 0, i;

    int StatoZona;
    byte mask = 1;

    val = getParm(cmdGetSensorStatus, sizeof(cmdGetSensorStatus), Rx, Count);

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
        if (StatoZona == 1)
        {
          val = true;
        }
        else
        {
          val = false;
        }
        if (zona[i] == nullptr)
        {
          zona[i] = new BinarySensor("Allarme Zona " + to_string(i));
        }
        zona[i]->publish_state(val);
      }

      // Ciclo SABOTAGGIO ZONE
      for (i = 0; i < 32; i++)
      {
        mask = 00000001;
        StatoZona = -1;
          
        if (i >= 24)
          StatoZona = (Rx[10] >> (i - 24)) & 1;
        else if (i >= 16 && i <= 23)
          StatoZona = (Rx[11] >> (i - 16)) & 1;
        else if (i >= 8 && i <= 15)
          StatoZona = (Rx[12] >> (i - 8)) & 1;
        else if (i <= 7)
          StatoZona = (Rx[13] >> i) & 1;

        if (StatoZona == 1)
        {
          val = true;
        }
        else
        {
          val = false;
        }
        if (zona_sabotaggio[i] == nullptr)
        {
          zona_sabotaggio[i] = new BinarySensor("Sabotaggio Zona "  + to_string(i));
        }
        zona_sabotaggio[i]->publish_state(val);
      }

      // Ciclo WARNINGS
      for (i = 0; i < 8; i++)
      {
        mask = 00000001;
        StatoZona = -1;
        StatoZona = (Rx[14] >> i) & 1;

        if (StatoZona == 1)
        {
          val = true;
        }
        else
        {
          val = false;
        }
        
        if(i == 0)
        {
          warn_mancanza_rete->publish_state(val);
        }
        else if(i == 1)
        {
          warn_scomparsa_bpi->publish_state(val);
        }
        else if(i == 2)
        {
          warn_fusibile->publish_state(val);
        }
        else if(i == 3)
        {
          warn_batteria_bassa->publish_state(val);
        }
        else if(i == 4)
        {
          warn_guasto_linea_telefonica->publish_state(val);
        }
        else if(i == 5)
        {
          warn_codici_default->publish_state(val);
        }
        else if(i == 6)
        {
          warn_wireless->publish_state(val);
        }
        
      }

      // Ciclo ALLARME AREA
      for (i = 0; i < 8; i++)
      {
        mask = 00000001;
        StatoZona = -1;
        StatoZona = (Rx[15] >> i) & 1;

        if (StatoZona == 1)
        {
          val = true;
        }
        else
        {
          val = false;
        }
        if (allarme_area[i] == nullptr)
        {
          allarme_area[i] = new BinarySensor("Allarme Area " + to_string(i));
        }
        allarme_area[i]->publish_state(val);
      }

      // Ciclo SABOTAGGI
      for (i = 0; i < 8; i++)
      {
        mask = 00000001;
        StatoZona = -1;
        StatoZona = (Rx[16] >> i) & 1;

        if (StatoZona == 1)
        {
          val = true;
        }
        else
        {
          val = false;
        }
        
        if(i == 2)
        {
          sabotaggio_zona->publish_state(val);
        }
        else if(i == 3)
        {
          sabotaggio_chiave_falsa->publish_state(val);
        }
        else if(i == 4)
        {
          sabotaggio_bpi->publish_state(val);
        }
        else if(i == 5)
        {
          sabotaggio_sistema->publish_state(val);
        }
        else if(i == 6)
        {
          sabotaggio_jam->publish_state(val);
        }
        else if(i == 7)
        {
          sabotaggio_wireless->publish_state(val);
        }
        
      }

    }
  }
};
