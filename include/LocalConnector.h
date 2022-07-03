#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

struct UdpData
{
    uint8_t cpuPercent;
    uint8_t memPercent;
    uint8_t disk0Percent;
    uint8_t disk1Percent;
    uint32_t diskReadRate;
    uint32_t diskWriteRate;
    uint32_t netSentRate;
    uint32_t netReceiveRate;
};

class LocalConnector
{
public:
    LocalConnector();
    void FetchNewData();
    UdpData* data;

private:
    WiFiUDP Udp;
};
