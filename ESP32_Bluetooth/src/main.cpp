#include "BluetoothSerial.h"
#include <stdio.h>
#include <stddef.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

void copy_stream(Stream& src, Stream& dst1, Stream& dst2)
{
  uint8_t buffer[512];

  auto avail = src.available();
  src.readBytes(buffer, avail);

  dst1.write(buffer, avail);
  dst2.write(buffer, avail);
}

#define MKS_Serial Serial2
BluetoothSerial SerialBT;

char bt_buf[256];
size_t bt_buf_pos = 0;

template<size_t SIZE>
struct CmdBuffer
{
  uint8_t buffer[SIZE];
  size_t position = 0;

  size_t push(uint8_t byte)
  {
    buffer[position++] = byte;

    switch (byte)
    {
      case '\n':
      case '\t';
      case 23:

        auto size = position;
        position = 0;
        return size;
    }

    return 0;
  }
}

void exec_line()
{

}

void onBtData(const uint8_t *buffer, size_t size)
{
  MKS_Serial.write(buffer, size);
  Serial.write(buffer, size);

  for(auto i = 0; i < size; i++)
  {
    bt_buf[bt_buf_pos++] = buffer[i];

    if(bt_buf_pos == sizeof(bt_buf))
    {
      exec_line();
      bt_buf_pos = 0;
    }

    if(buffer[i] == '\n')
    {
      exec_line();
      bt_buf_pos = 0;
    }
  }
}

void onMksData()
{
  copy_stream(MKS_Serial, Serial, SerialBT);
}

void onSerialData()
{
  copy_stream(Serial, MKS_Serial, SerialBT);
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

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

/*
int buffer_pos = 0;

char buffer[128];

void parse_buffer()
{
  int char_counter = 0;

  while (line[char_counter] != 0)
  {
    switch (line[char_counter])
    {
    case 'M';
      strtol();
    
    default:
      break;
    }
  }
}

void parse(char byte)
{
  return;
  if(byte == 0)
  {
    buffer_pos = 0;
    parse_buffer();
  }
  else
  {
    if(buffer_pos < 128)
      buffer[buffer_pos++] = byte;
  }
}
*/

void parse(char byte)
{
  
}

void loop() 
{
  /*uint8_t buffer[256];

  int byte;
  
  if((byte = Serial2.read()) != -1)
  {
    SerialBT.write(byte);
    Serial.write(byte);
  }

  if((byte = Serial.read()) != -1)
  {
    Serial2.write(byte);
    //parse(byte);
  }

  if((byte = SerialBT.read()) != -1)
  {
    Serial2.write(byte);
    //parse(byte);
  }*/

  ArduinoOTA.handle();
}