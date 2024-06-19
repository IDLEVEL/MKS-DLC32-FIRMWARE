#pragma once

#include "helpers.h"
#include "BluetoothSerial.h"
#include "ESPTelnet.h"

extern CmdBuffer<512> bt_buffer;
extern CmdBuffer<512> mks_buffer;

#define MKS_Serial Serial2

extern BluetoothSerial SerialBT;
extern ESPTelnet Telnet;