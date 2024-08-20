#pragma once

#include "Grbl_bluetooth.h"
#include "GCodeCommand.h"

class GCodeParser
{
    GCodeCommand _command;

    const char* _current_str_ptr;

    CLIENT _client;

    bool parse_command_m();
    bool parse_command_internal();
    
    bool _parse_internal();

    const char PARSE_ERROR = 0;
	
	Stream _stream;
	
	struct Stream
	{
		const char* _str;
		uint16_t _length;
		uint16_t _position;
		
		bool read_number(float& num)
		{
			
		}

		bool read_char(char& ch)
		{
			if(_position < length)
			{
				ch = _str[_position++];
				return true;
			}
			
			return false;
		}
		
		void skip_chars(char ch)
		{
			while(_str[_position] == ch && _position < _length)
				_position++;
		}
		
		void skip_spaces()
		{
			return skip_chars(' ');
		}
		
		bool try_read(const char* str)
		{
			while(*str != '\0')
			{
				if(_position == _length)
					return false;
				
				if(*str != _str[_position])
					return false;
				
				str++;
				_position++;
			}
			
			return true;
		}
	};

public:

    GCodeParser(CLIENT client, const char* str, uint16_t length) : _client(client), _str(str), _length(length), _position(0)
    {

    }
	
	bool go_next_token()
	{
		while(*_current_str != 0)
		{
			if(*_current_str == ' ')
			{
				_current_str++;
			
				return true;
			}
		}
		
		return false;
	}
	
    bool read_int(int32_t& value, uint8_t min_value, uint8_t max_value = 0, bool can_negative = false)
    {
		bool negative = false;

		auto start_pos = char_ptr;
		auto char_ptr = _current_str;
		
		if(*char_ptr == 45)
		{
			if(!can_negative)
				return false;
			
			negative = true;
			start_pos++;
			char_ptr++;
		}
		
        while(*char_ptr != 0 && (char_val >= '0' && char_val <= '9'))
			char_ptr++;
		
		if(char_ptr == start_pos)
			return false;
			
		auto end_ptr = char_ptr;
		
		uint32_t result = 0;
		uint32_t position = 1;
		
		while(char_ptr != start_pos)
		{
			char char_val = *char_ptr;

			if(char_val >= '0' && char_val <= '9')
			{
				uint32_t digit_value = uint32_t(char_val - '0');
	
				result = result + (position * digit_value);
				position = position * 10;
				
				if(result > max_value)
					return false;
			}
			
			char_ptr--;
		}
		
		if(result < min_value)
			return false;
		
		_current_str = end_ptr - 1;

		value = negative ? -int32_t(result) : result;
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