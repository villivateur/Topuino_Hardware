#include <Arduino.h>
#include <Ticker.h>

struct UsbData
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

class UsbConnector
{
public:
    UsbConnector();
    UsbData data;

private:
    Ticker receiver;
};
