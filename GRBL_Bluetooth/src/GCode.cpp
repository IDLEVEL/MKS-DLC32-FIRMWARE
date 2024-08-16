#include "GCode.h"
#include "WiFi.h"

#define responce(client, ...) \
    snprintf(buffer, sizeof(buffer), #__VA_ARGS__); \
    this->write(client, buffer);

bool GCodeParser::_parse_internal()
{
    _current_str_ptr = _str;
	
	if(strcmp(_str + _position, "OK\n") == 0)
	{
		_command = {GCodeCommand::OK};
        return false;
	}

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

bool GCodeParser::parse_command_m()
{
<<<<<<< HEAD
    switch(parse_char())
=======
    switch(read_int())
>>>>>>> ecbc0b326040a0e1723733a326971da189042def
    {
        case PARSE_ERROR:
            return false;

        case 42:

            if(!next())
                return false;
			
			if(read_char() == 'I')
			{
				if(!next())
					return false;
			}
			
			if(read_char() == 'P')
			{
				if(!read_int(pin))
					return false;
				
				if(!next())
					return true;
			}
			
			if(read_char() == 'S')
			{
				if(!read_int(pwm))
					return false;
			}
			
			return true;
			
		case 42:

            if(!next())
                return false;
			
			if(read_char() == 'I')
			{
				if(!next())
					return false;
			}
			
			if(read_char() == 'P')
			{
				if(!read_int(pin))
					return false;
				
				if(!next())
					return true;
			}
			
			if(read_char() == 'S')
			{
				if(!read_int(pwm))
					return false;
			}
			
			return true;
			
			//G1 X30 Y15 Z2 F100
			
			while(next())
			{
				switch(read_char())
				{
					'X':
					
						if(!read_decimal(pwm))
							return false;
						
						break;
						
					'Y':
					
						if(!read_decimal(pwm))
							return false;
						
						break;
						
					'Z':
					
						if(!read_decimal(pwm))
							return false;
						
						break;
						
					'F':
					
						if(!read_decimal(pwm))
							return false;
						
						break;
				}
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

    if(strcmp(_str + _position, "TOOL_CHANGE") == 0)
    {
        _command.cmd = GCodeCommand::TOOL_CHANGE;
        return false;
    }

    _command = {GCodeCommand::OTHER};

    return true;
}

bool GCodeExcec::execute(GCodeCommand cmd)
{
    char buffer[256];

    if(cmd.is_client(CLIENT_INPUT))
    {
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

            case GCodeCommand::TOOL_CHANGE:
                tool_change_state = TOOL_CHANGE_WAIT_TOOL;
                return true;
        }
    }
    else if(cmd.is_client(CLIENT_BOARD))
    {
        switch(tool_change_state)
        {
            case TOOL_CHANGE_WAIT_TOOL:

                if(cmd.cmd == GCodeCommand::SUCCESS)
                {
                    tool_change_state = TOOL_CHANGE_WAIT_PROBE;
                    send_command(CLIENT_BOARD, "M01");
                }

            case TOOL_CHANGE_WAIT_PROBE:
                 
                if(cmd.cmd == GCodeCommand::SUCCESS)
                {
                    tool_change_state = TOOL_CHANGE_WAIT_PROBE;
                    send_command(CLIENT_BOARD, "PROBE");
                }
				
			case TOOL_CHANGE_WAIT_PROBE:
        }
    }
    
    return false;
}