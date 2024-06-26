#include "GCode.h"
#include "WiFi.h"

#define responce(client, ...) \
    snprintf(buffer, sizeof(buffer), #__VA_ARGS__); \
    this->write(client, buffer);

bool GCodeParser::_parse_internal()
{
    switch (_str[_position++])
    {
    case '#':
        return parse_command_internal();

    case 'M':
        return parse_command_m();
    
    default:
        return true;
    }
}

#define PARSE_ERROR 100500

int parse_int()
{

}

int next_token()
{

}

bool GCodeParser::parse_command_m()
{
    switch(parse_int())
    {
        case PARSE_ERROR:
            return true;

        case 42:

            if(next_token() == PARSE_ERROR)
                return true;

            switch(parse_char())
            {
                case 'I':
                    //ignore

                case 'P':
                    int8_t pin = parse_int();

                case 'S':
                    int8_t pwm = parse_int();

                case 'T':
                    //pulldown
            }
    }
}

bool GCodeParser::parse_command_internal()
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

bool GCodeExcec::execute(GCodeCommand cmd)
{
    char buffer[256];

    switch(cmd.cmd)
    {
        case GCodeCommand::RESTART:
            esp_restart();
            return true;
        
        case GCodeCommand::GET_IP:
            responce(CLIENT_ALL_OUTPUT, "IP = %s" WiFi.localIP().toString());
            return true;
            
        case GCodeCommand::SET_WIFI_SSID:
            responce(CLIENT_ALL_OUTPUT, "new WIFI ssid = %s" cmd.WIFI_SSID);
            return true;

        case GCodeCommand::SET_WIFI_PASS:
            auto ip_string = WiFi.localIP();
            responce(CLIENT_ALL_OUTPUT, "new WIFI password = %s" cmd.WIFI_PASS);
            return true;
    }
    
    return false;
}