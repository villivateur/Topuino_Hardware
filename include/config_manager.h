#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class ConfigManager
{
private:
    IPAddress localIp;
    IPAddress gateway;
    IPAddress subnet;
public:
    ESP8266WebServer httpServer;
    ConfigManager();
    void ProcessConfig();
};
