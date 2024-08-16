#pragma once

#include "Grbl_bluetooth.h"

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
        NONE,
        SUCCESS,
        GET_IP,
        SET_WIFI_SSID,
        SET_WIFI_PASS,
        SET_PIN,
        RESTART,
        TOOL_CHANGE,
        OTHER

    } cmd;

    CLIENT client;

    union
    {
		struct WIFI
		{
			char WIFI_SSID[24];
			char WIFI_PASS[24];
		};
		
		WIFI wifi;
		
        uint8_t pin_id;
        uint8_t pin_pwm;
    };

    bool is_client(CLIENT client)
    {
        return (client & client);
    }
};
