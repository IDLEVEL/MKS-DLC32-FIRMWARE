#pragma once

#include "Grbl_bluetooth.h"
#include "GCodeCommand.h"
#include "GCodeParser.h"

class GCodeExcec
{
    typedef enum
    {
        TOOL_CHANGE_NONE,
        TOOL_CHANGE_WAIT_TOOL,
        TOOL_CHANGE_WAIT_PROBE

    } tool_change_state;

public:

    void write(CLIENT client, const char* buffer)
    {

    }

    bool execute(GCodeCommand cmd);

    void send_command(CLIENT client, const char* cmd)
    {
		if(client & CLIENT::CLIENT_BT)
        {
            SerialBT.write(ptr, size);
        }
        else if(client & CLIENT::CLIENT_BOARD)
        {
            MKS_Serial.write(ptr, size);
        }
    }

    void process(CLIENT client, const uint8_t* ptr, uint16_t size)
    {
        if(client == CLIENT::CLIENT_BOARD)
        {
             auto gcode_parser = GCodeParser(client, (char*)ptr, size);

            if(gcode_parser.parse())
            {
                execute(gcode_parser.get_command());
            }

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