#pragma once
#include "Grbl.h"

void i2c_init();
void i2c_loop();

int i2c_read();

void i2c_write(const char* text);

int i2c_available();