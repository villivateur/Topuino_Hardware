#include "DisplayPanel.h"

#define STROBE_TM1 13
#define STROBE_TM2 12 // strobe = GPIO connected to strobe line of module
#define STROBE_TM3 14
#define CLOCK_TM 4      // clock = GPIO connected to clock line of module
#define DIO_TM 5        // data = GPIO connected to data line of module
#define HIGH_FREQ false //default false, If using a high freq CPU > ~100 MHZ set to true.
#define LED_BRIGHTNESS 1

DisplayPanel::DisplayPanel()
{
    // Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
    percentLed = new TM1638plus(STROBE_TM1, CLOCK_TM, DIO_TM, HIGH_FREQ);
    diskIoLed = new TM1638plus(STROBE_TM2, CLOCK_TM, DIO_TM, HIGH_FREQ);
    netIoLed = new TM1638plus(STROBE_TM3, CLOCK_TM, DIO_TM, HIGH_FREQ);
    percentLed->displayBegin();
    diskIoLed->displayBegin();
    netIoLed->displayBegin();
    percentLed->brightness(LED_BRIGHTNESS);
    diskIoLed->brightness(LED_BRIGHTNESS);
    netIoLed->brightness(LED_BRIGHTNESS);
}

uint16_t DisplayPanel::PercentToBitmap(uint8_t percent)
{
    uint32_t tempPercentInt;
    uint8_t tempPercentRem;
    uint16_t tempDisplayBit;

    tempPercentInt = percent * 15 / 100;
    tempPercentRem = percent * 15 % 100;
    if (tempPercentRem > 50) {
        tempPercentInt += 1;
    }
    tempDisplayBit = 0x1;
    for (uint8_t i = 0; i < tempPercentInt; i++) {
        tempDisplayBit <<= 1;
        tempDisplayBit += 1;
    }

    return tempDisplayBit;
}

void DisplayPanel::DisplayCpuPercent(uint8_t percent)
{
    uint16_t bitmap = PercentToBitmap(percent);
    percentLed->display7Seg(0, bitmap % 0x100);
    percentLed->display7Seg(1, bitmap / 0x100);
}

void DisplayPanel::DisplayMemPercent(uint8_t percent)
{
    uint16_t bitmap = PercentToBitmap(percent);
    percentLed->display7Seg(2, bitmap % 0x100);
    percentLed->display7Seg(3, bitmap / 0x100);
}

void DisplayPanel::DisplayDisk0Percent(uint8_t percent)
{
    uint16_t bitmap = PercentToBitmap(percent);
    percentLed->display7Seg(4, bitmap % 0x100);
    percentLed->display7Seg(5, bitmap / 0x100);
}

void DisplayPanel::DisplayDisk1Percent(uint8_t percent)
{
    uint16_t bitmap = PercentToBitmap(percent);
    percentLed->display7Seg(6, bitmap % 0x100);
    percentLed->display7Seg(7, bitmap / 0x100);
}

void DisplayPanel::DisplayDiskRate(uint32_t byteRdPerSec, uint32_t byteWrPerSec)
{
    uint16_t displayRd;
    uint16_t displayWr;
    char displayStr[9];

    if (byteRdPerSec < 1024 * 1024) {
        displayRd = byteRdPerSec / 1024;
        diskIoLed->setLED(0, 0);
        diskIoLed->setLED(1, 1);
    } else {
        displayRd = byteRdPerSec / 1024 / 1024;
        diskIoLed->setLED(0, 1);
        diskIoLed->setLED(1, 0);
    }
    if (byteWrPerSec < 1024 * 1024) {
        displayWr = byteWrPerSec / 1024;
        diskIoLed->setLED(2, 0);
        diskIoLed->setLED(3, 1);
    } else {
        displayWr = byteWrPerSec / 1024 / 1024;
        diskIoLed->setLED(2, 1);
        diskIoLed->setLED(3, 0);
    }
    sprintf(displayStr, "%4u%4u", displayRd, displayWr);
    diskIoLed->displayText(displayStr);
}

void DisplayPanel::DisplayNetRate(uint32_t byteTxPerSec, uint32_t byteRxPerSec)
{
    uint16_t displayTx;
    uint16_t displayRx;
    char displayStr[9];

    if (byteTxPerSec < 1024 * 1024) {
        displayTx = byteTxPerSec / 1024;
        netIoLed->setLED(0, 0);
        netIoLed->setLED(1, 1);
    } else {
        displayTx = byteTxPerSec / 1024 / 1024;
        netIoLed->setLED(0, 1);
        netIoLed->setLED(1, 0);
    }
    if (byteRxPerSec < 1024 * 1024) {
        displayRx = byteRxPerSec / 1024;
        netIoLed->setLED(2, 0);
        netIoLed->setLED(3, 1);
    } else {
        displayRx = byteRxPerSec / 1024 / 1024;
        netIoLed->setLED(2, 1);
        netIoLed->setLED(3, 0);
    }
    sprintf(displayStr, "%4u%4u", displayTx, displayRx);
    netIoLed->displayText(displayStr);
}
