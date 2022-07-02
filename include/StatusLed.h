#include <Arduino.h>
#include <Ticker.h>

#define STATUS_LED_PIN 2

class StatusLed
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
    StatusLed();
    void SetBlinkRate(BlinkRate rate);
};