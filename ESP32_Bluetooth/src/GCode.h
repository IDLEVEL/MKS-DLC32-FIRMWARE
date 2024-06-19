#pragma once

#include <stdint.h>
#include <string.h>

struct GCodeCommand
{
    enum
    {
        GET_IP

    } cmd;
};

class GCodeParser
{
    using size_type = uint16_t;

    const char* _str;
    size_type _length;
    size_type _position;
    GCodeCommand _command;

    bool parse_X()
    {
        if(strcmp(_str + _position, "IP") == 0)
        {
            _command = {GCodeCommand::GET_IP};
            return false;
        }

        return true;
    }

public:

    GCodeParser(const char* str, size_type length) : _str(str), _length(length), _position(0)
    {

    }

    bool parse()
    {
        switch (_str[_position++])
        {
        case 'X':
            return parse_X();
        
        default:
            return true;
        }
    }

};

class GCodeExcec
{
public:

    void execute(GCodeCommand cmd)
    {
        
    }
};