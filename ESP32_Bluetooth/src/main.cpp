#include "BluetoothSerial.h"
#include <stdio.h>
#include <stddef.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define I2C_DEV_ADDR 0x55
BluetoothSerial SerialBT;

void setup() {

  Serial.begin(115200);

  SerialBT.begin("MKS32"); //Bluetooth device name
  
  Serial2.begin(115200, SERIAL_8N1, SDA, SCL);

  
  //Wire.onReceive(onReceive);
  //Wire.onRequest(onRequest);

  //Wire.begin((uint8_t)I2C_DEV_ADDR, SDA, SCL, 100000);
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
  int byte = 0;

  if((byte = Serial2.read()) != -1)
  {
    SerialBT.write(byte);
    Serial.write(byte);
  }

  if((byte = Serial.read()) != -1)
  {
    Serial2.write(byte);
    parse(byte);
  }

  if((byte = SerialBT.read()) != -1)
  {
    Serial2.write(byte);
    parse(byte);
  }
}