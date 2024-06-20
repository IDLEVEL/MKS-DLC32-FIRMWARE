#pragma once

#include "Grbl_bluetooth.h"
#include "GCodeCommand.h"
#include "GCodeParser.h"

class GCodeExcec
{
public:

    void write(CLIENT client, const char* buffer)
    {

    }

    bool execute(GCodeCommand cmd);

    void process(CLIENT client, const uint8_t* ptr, uint16_t size)
    {
        if(client == CLIENT::CLIENT_BOARD)
        {
            SerialBT.write(ptr, size);
        }
        else
        {
            auto gcode_parser = GCodeParser(client, (char*)ptr, size);

            if(gcode_parser.parse())
            {
                execute(gcode_parser.get_command());
            }
            else
            {
                MKS_Serial.write(ptr, size);
            }
        }
    }
};