#include <Arduino.h>
#include <Ticker.h>

#define STATUS_LED_PIN 2

class StatusBlink
{
private:
    Ticker flipper;

public:
    enum BlinkRate {
        RateAlwaysOn,
        Rate5Hz,
        Rate2Hz,
        Rate0_5Hz,
        RateAlwaysOff,
    };
    StatusBlink();
    void SetBlinkRate(BlinkRate rate);
};