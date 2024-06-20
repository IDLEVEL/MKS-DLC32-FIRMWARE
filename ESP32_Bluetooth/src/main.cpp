
#include <stdio.h>
#include <stddef.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>
#include "helpers.h"
#include "GCode.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
ESPTelnet Telnet;

CmdBuffer<512> bt_buffer;
CmdBuffer<512> mks_buffer;

GCodeExcec gcode_excec;

void onBtData(const uint8_t *buffer, size_t size)
{
  uint16_t used_size = 0;

  for(auto i = 0; i < size; i++)
  {
    if((used_size = bt_buffer.push(buffer[i])) != CMD_BUFFER_WAIT)
    {
      gcode_excec.process(CLIENT_BT, (char*)bt_buffer.buffer, used_size);
    }
  }

  //MKS_Serial.write(buffer, size);
}

void onMksData()
{
  uint16_t used_size = 0;

  while(MKS_Serial.available())
  {
    auto byte = MKS_Serial.read();

    if(byte < 0)
      break;

    if((used_size = mks_buffer.push(byte)) != CMD_BUFFER_WAIT)
    {
      gcode_excec.process(CLIENT_BOARD, (char*)mks_buffer.buffer, used_size);
    }    
  }
}

void onSerialData()
{
  //copy_stream(Serial, MKS_Serial, SerialBT);
}

const char *ssid = "kv-41";
const char *password = "21302130";

void setup() 
{

  Serial.begin(115200);

  SerialBT.begin("MKS32"); //Bluetooth device name

  MKS_Serial.begin(115200, SERIAL_8N1);

  SerialBT.onData(onBtData);
  Serial.onReceive(onSerialData);
  MKS_Serial.onReceive(onMksData);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  for(auto i = 0; i < 20; i++)
  {
    if(WiFi.waitForConnectResult() == WL_CONNECTED)
      break;

    delay(100);
  }

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else {  // U_SPIFFS
        type = "filesystem";
      }

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
      } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
      } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
      } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
      } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
      }
    });

  ArduinoOTA.begin();

  telnet.begin();   

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() 
{
  telnet.loop();

  delay(3);

  ArduinoOTA.handle();

  delay(3);
}