#pragma once

#include "Grbl_bluetooth.h"
#include "GCodeCommand.h"

class GCodeParser
{
    const char* _str;
    uint16_t _length;
    uint16_t _position;
    GCodeCommand _command;

    CLIENT _client;

    bool parse_command_m();
    bool parse_command_internal();
    
    bool _parse_internal();

public:

    GCodeParser(CLIENT client, const char* str, uint16_t length) : _client(client), _str(str), _length(length), _position(0)
    {

    }

    u32_t read_int()
    {
        
    }

    bool parse()
    {
        auto success = _parse_internal();

        if(success)
            _command.client = _client;

        return success;
    }

    const GCodeCommand& get_command()
    {
        return _command;
    }
};