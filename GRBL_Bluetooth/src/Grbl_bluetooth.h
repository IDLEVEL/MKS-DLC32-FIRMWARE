#pragma once

#include "helpers.h"
#include "BluetoothSerial.h"
#include "ESPTelnet.h"
#include <stdint.h>

#define MKS_Serial Serial2

extern BluetoothSerial SerialBT;
extern ESPTelnet Telnet;