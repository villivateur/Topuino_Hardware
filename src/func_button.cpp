#include "func_button.h"
#include "user_data.h"

#define FUNC_BTN 0
#define PRESSED_STATUS LOW

extern UserData* userdataManager;

static int pressedTime;

static void Scan()
{
    if (digitalRead(FUNC_BTN) == PRESSED_STATUS) {
        pressedTime++;
        return;
    }

    int count = pressedTime;
    pressedTime = 0;
    
    if (count >= 10) {
        // Factory reset
        userdataManager->EraseData();
        delay(1000);
        ESP.restart();
        return;
    }

    if (count >= 2) {
        return;
    }
}

FuncButton::FuncButton()
{    
    pinMode(FUNC_BTN, INPUT);
    pressedTime = 0;
    
    scanner.attach(1.0, Scan);
}
