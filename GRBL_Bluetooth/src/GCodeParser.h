#pragma once

#include "Grbl_bluetooth.h"
#include "GCodeCommand.h"

class GCodeParser
{
    const char* _str;
    uint16_t _length;
    uint16_t _position;
    GCodeCommand _command;

    const char* _current_str_ptr;

    CLIENT _client;

    bool parse_command_m();
    bool parse_command_internal();
    
    bool _parse_internal();

    const char PARSE_ERROR = 0;

public:

    GCodeParser(CLIENT client, const char* str, uint16_t length) : _client(client), _str(str), _length(length), _position(0)
    {

    }

    bool next_token()
    {
        return true;
    }

    char parse_char()
    {
        if(*_current_str_ptr == 0)
            return PARSE_ERROR;

        return *_current_str_ptr;
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