#pragma once

#include <stdint.h>
#include <string.h>
#include "esp32_bluetooth.h"

enum CLIENT
{
    CLIENT_BT = 0x1,
    CLIENT_BOARD = 0x2,
    CLIENT_TELNET = 0x4,
    CLIENT_SERIAL = 0x8,

    CLIENT_ALL_OUTPUT = CLIENT_BT | CLIENT_TELNET | CLIENT_SERIAL
};

struct GCodeCommand
{
    enum
    {
        GET_IP,
        SET_WIFI_SSID,
        SET_WIFI_PASS,
        SET_PIN,
        RESTART

    } cmd;

    CLIENT client;

    union
    {
        char WIFI_SSID[24];
        char WIFI_PASS[24];

        uint8_t pin_id;
        uint8_t pin_pwm;
    };

    bool is_client(CLIENT client)
    {
        return (client & client);
    }
};

class GCodeParser
{
    using size_type = uint16_t;

    const char* _str;
    size_type _length;
    size_type _position;
    GCodeCommand _command;

    CLIENT _client;

    bool parse_q()
    {
        if(strcmp(_str + _position, "IP") == 0)
        {
            _command = {GCodeCommand::GET_IP};
            return false;
        }

        if(strcmp(_str + _position, "RESTART") == 0)
        {
            _command = {GCodeCommand::RESTART};
            return false;
        }

        if(sscanf("PIN %d %d", _str + _position, &_command.pin_id, &_command.pin_pwm))
        {
            _command.cmd = GCodeCommand::SET_PIN;
            return false;
        }

        if(strcmp(_str + _position, "WIFI ") == 0)
        {
            if(sscanf("SSID %s", _str + _position + 5, _command.WIFI_SSID))
            {
                _command.cmd = GCodeCommand::SET_WIFI_SSID;
                return false;
            }

            if(sscanf("PASS %s", _str + _position + 5, _command.WIFI_PASS))
            {
                _command.cmd = GCodeCommand::SET_WIFI_PASS;
                return false;
            }
        }

        return true;
    }

public:

    GCodeParser(const char* str, size_type length) : _str(str), _length(length), _position(0)
    {

    }

    bool _parse_internal()
    {
        switch (_str[_position++])
        {
        case '?':
            return parse_q();
        
        default:
            return true;
        }
    }

    bool parse()
    {
        auto success = _parse_internal();

        if(success)
            _command.client = _client;

        return success;
    }

};

class GCodeExcec
{
    GCodeParser _parser;
    
public:

    void write(CLIENT client, const char* buffer)
    {

    }

    bool execute(GCodeCommand cmd);

    void process(CLIENT cleint, const char* ptr, GCodeParser::size_type size)
    {
        if(!_parser.parse())
        {
            switch (client)
            {
            case CLIENT::CLIENT_BOARD:
                SerialBT.write(ptr, size);
                break;

            case CLIENT::CLIENT_BT:
                MKS_Serial.write(ptr, size);
                break;
            
            default:
                break;
            }
        }
    }
};