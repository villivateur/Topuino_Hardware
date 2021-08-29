#include <Arduino.h>

class FuncButton
{
private:
    uint8_t pressedTime;

public:
    FuncButton();
    void Scan();
};
