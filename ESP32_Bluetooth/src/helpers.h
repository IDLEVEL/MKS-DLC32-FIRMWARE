#pragma once

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#define CMD_BUFFER_WAIT USHRT_MAX

template<uint16_t SIZE>
struct CmdBuffer
{
  uint8_t buffer[SIZE];
  uint16_t position = 0;

  uint8_t* ptr()
  {
    return buffer;
  }

  uint16_t push(uint8_t byte)
  {
    buffer[position++] = byte;

    if(position >= (SIZE - 1))
    {
        auto size = position;
        buffer[size] = 0;

        position = 0;
        return size;
    }

    switch(byte)
    {
        case '\r':
        case '\n':
        case '?':
        case '%':
        case 24:
            auto size = position;
            buffer[size] = 0;
            position = 0;
            return size;
    }

    return CMD_BUFFER_WAIT;
  }
};