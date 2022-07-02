#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class WifiConfigManager
{
private:
    IPAddress localIp;
    IPAddress gateway;
    IPAddress subnet;
public:
    ESP8266WebServer httpServer;
    WifiConfigManager();
    void ProcessConfig();
};
