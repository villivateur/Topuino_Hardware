#include <Arduino.h>
#include <Ticker.h>

class FuncButton
{
private:
    uint8_t pressedTime;
    Ticker scanner;

public:
    FuncButton();
};
