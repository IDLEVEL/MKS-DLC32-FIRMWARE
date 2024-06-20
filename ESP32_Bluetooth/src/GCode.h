#pragma once

#include <stdint.h>
#include <string.h>
#include "esp32_bluetooth.h"

enum CLIENT : uint8_t
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
    const char* _str;
    uint16_t _length;
    uint16_t _position;
    GCodeCommand _command;

    CLIENT _client;

    bool parse_command()
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

    GCodeParser(CLIENT client, const char* str, uint16_t length) : _client(client), _str(str), _length(length), _position(0)
    {

    }

    bool _parse_internal()
    {
        switch (_str[_position++])
        {
        case '#':
            return parse_command();
        
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

    const GCodeCommand& get_command()
    {
        return _command;
    }
};

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