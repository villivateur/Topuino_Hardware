#include <Arduino.h>
#include <TM1638plus.h>

class DisplayPanel
{
private:
    uint16_t PercentToBitmap(uint8_t percent);
    TM1638plus* percentLed;
    TM1638plus* diskIoLed;
    TM1638plus* netIoLed;

public:
    DisplayPanel();
    void DisplayCpuPercent(uint8_t percent);
    void DisplayMemPercent(uint8_t percent);
    void DisplayDisk0Percent(uint8_t percent);
    void DisplayDisk1Percent(uint8_t percent);
    void DisplayDiskRate(uint32_t byteRdPerSec, uint32_t byteWrPerSec);
    void DisplayNetRate(uint32_t byteTxPerSec, uint32_t byteRxPerSec);
};