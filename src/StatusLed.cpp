#include "StatusLed.h"

StatusLed::StatusLed()
{
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH);
}

static void BlinkTask()
{
    digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));     // set pin to the opposite state
}

void StatusLed::SetBlinkRate(BlinkRate rate)
{
    switch (rate)
    {
    case BlinkRate::RateAlwaysOn:
        flipper.detach();
        digitalWrite(STATUS_LED_PIN, LOW);
        break;
    case BlinkRate::RateAlwaysOff:
        flipper.detach();
        digitalWrite(STATUS_LED_PIN, HIGH);
        break;
    case BlinkRate::Rate5Hz:
        flipper.attach(0.1, BlinkTask);        
        break;
    case BlinkRate::Rate2Hz:
        flipper.attach(0.25, BlinkTask);        
        break;
    case BlinkRate::Rate0_5Hz:
        flipper.attach(1.0, BlinkTask);        
        break;
    }
}