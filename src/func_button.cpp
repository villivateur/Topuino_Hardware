#include "func_button.h"
#include "user_data.h"

#define FUNC_BTN 0
#define PRESSED_STATUS LOW

extern UserData* userdataManager;

FuncButton::FuncButton()
{
    pinMode(FUNC_BTN, INPUT);
    pressedTime = 0;
}

void FuncButton::Scan()
{
    if (digitalRead(FUNC_BTN) == PRESSED_STATUS) {
        pressedTime++;
    } else {
        pressedTime = 0;
    }
    if (pressedTime > 5) {
        userdataManager->EraseData();
        ESP.restart();
    }
}