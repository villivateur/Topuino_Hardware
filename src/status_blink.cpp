#include "status_blink.h"

StatusBlink::StatusBlink()
{
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH);
}

static void BlinkTask()
{
    digitalWrite(STATUS_LED_PIN, !digitalRead(STATUS_LED_PIN));     // set pin to the opposite state
}

void StatusBlink::SetBlinkRate(BlinkRate rate)
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
    case BlinkRate::Rate8Hz:
        flipper.attach(0.125, BlinkTask);        
        break;
    case BlinkRate::Rate2Hz:
        flipper.attach(0.5, BlinkTask);        
        break;
    case BlinkRate::Rate0_5Hz:
        flipper.attach(2.0, BlinkTask);        
        break;
    }
}