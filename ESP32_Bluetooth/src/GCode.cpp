#include "GCode.h"
#include "WiFi.h"

#define responce(client, ...) \
    snprintf(buffer, sizeof(buffer), #__VA_ARGS__); \
    this->write(client, buffer);

bool GCodeExcec::execute(GCodeCommand cmd)
{
    char buffer[256];

    switch(cmd.cmd)
    {
        case GCodeCommand::RESTART:
            esp_restart();
            return true;
        
        case GCodeCommand::GET_IP:
            auto ip_string = WiFi.localIP();
            responce(CLIENT_ALL_OUTPUT, "IP = %s" ip_string.toString());
            return true;
            
        case GCodeCommand::SET_WIFI_SSID:
            auto ip_string = WiFi.localIP();
            responce(CLIENT_ALL_OUTPUT, "new WIFI ssid = %s" cmd.WIFI_SSID);
            return true;

        case GCodeCommand::SET_WIFI_PASS:
            auto ip_string = WiFi.localIP();
            responce(CLIENT_ALL_OUTPUT, "new WIFI password = %s" cmd.WIFI_PASS);
            return true;
    }
    
    return false;
}