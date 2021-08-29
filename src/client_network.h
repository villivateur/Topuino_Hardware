#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class ClientNetwork
{
private:
    StaticJsonDocument<512> receivedData;
    HTTPClient* http;
    STATUS status;

public:
    void Init();
    STATUS FetchNewData();
    uint8_t GetPercent(String name);
    uint32_t GetRate(String name);
};