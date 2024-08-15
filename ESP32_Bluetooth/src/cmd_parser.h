
struct GCodeCmd
{
	enum : uint8_t
	{
		PIN_ON,
		PIN_OFF,
		PIN_PWM,
		GET_IP,
		GET_TIME,
		STATE_SAVE
	};
	
	uint8_t cmd;
	
	struct
	{
		uint8_t pin;
		uint8_t pwm;
		
	} pin;
	
};
	
struct GCodePaser
{
	uint16_t pos = 0;
	const char* str = 0;
	
	GCodeCmd cmd;
		
	bool parse_M
	{
		switch(parse_int(str + pos))
		{
			case 23:
				cmd.cmd = GCodeCmd::PIN_ON;
				cmd.pin.pin = pin;
				return false;
		}
	}
	
	bool parse_I()
	{
		char buf[8];
		
		scanf(str, "I[%s]", buf);
		
		if(strcmp(buf, "IP") === 0)
		{
			cmd.cmd = GCodeCmd::GET_IP;
			return false;
		}
		
		if(strcmp(buf, "TIME") === 0)
		{
			cmd.cmd = GCodeCmd::GET_TIME;
			return false;
		}
		
		return true;
	}

	bool parse()
	{
		switch(str[pos++])
		{
			case 'M':
				return parseM();
				
			case 'I':
		}
	}
};

struct GCodeExec
{
	bool save_state = false;
	
	char responce[32];
	
	void exec(GCodeCmd cmd)
	{
		switch(cmd.cmd)
		{
			case GCodeCmd::PIN_ON:
				strcpy(responce, "ok\n");
				break;
			
			case GCodeCmd::GET_TIME:
				sprintf(responce, "%i\n", micros());
				break;
		}
	}
};