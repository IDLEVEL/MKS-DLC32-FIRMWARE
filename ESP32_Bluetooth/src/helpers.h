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

    switch((char)byte)
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

const int MAX_INT_DIGITS = 8;

uint8_t read_float(const char* line, uint8_t* char_counter, float* float_ptr) {
    const char*   ptr = line + *char_counter;
    unsigned char c;
    // Grab first character and increment pointer. No spaces assumed in line.
    c = *ptr++;
    // Capture initial positive/minus character
    bool isnegative = false;
    if (c == '-') {
        isnegative = true;
        c          = *ptr++;
    } else if (c == '+') {
        c = *ptr++;
    }

    // Extract number into fast integer. Track decimal in terms of exponent value.
    uint32_t intval    = 0;
    int8_t   exp       = 0;
    uint8_t  ndigit    = 0;
    bool     isdecimal = false;
    while (1) {
        c -= '0';
        if (c <= 9) {
            ndigit++;
            if (ndigit <= MAX_INT_DIGITS) {
                if (isdecimal) {
                    exp--;
                }
                intval = intval * 10 + c;
            } else {
                if (!(isdecimal)) {
                    exp++;  // Drop overflow digits
                }
            }
        } else if (c == (('.' - '0') & 0xff) && !(isdecimal)) {
            isdecimal = true;
        } else {
            break;
        }
        c = *ptr++;
    }
    // Return if no digits have been read.
    if (!ndigit) {
        return false;
    }

    // Convert integer into floating point.
    float fval;
    fval = (float)intval;
    // Apply decimal. Should perform no more than two floating point multiplications for the
    // expected range of E0 to E-4.
    if (fval != 0) {
        while (exp <= -2) {
            fval *= 0.01;
            exp += 2;
        }
        if (exp < 0) {
            fval *= 0.1;
        } else if (exp > 0) {
            do {
                fval *= 10.0;
            } while (--exp > 0);
        }
    }
    // Assign floating point value with correct sign.
    if (isnegative) {
        *float_ptr = -fval;
    } else {
        *float_ptr = fval;
    }
    *char_counter = ptr - line - 1;  // Set char_counter to next statement
    return true;
}