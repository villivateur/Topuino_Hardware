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
        userdataManager->EraseAllData();
        Serial.end();
        delay(1000);
        ESP.restart();
        return;
    }

    if (count >= 2) {
        RunMode mode = userdataManager->GetRunMode();
        switch (mode)
        {
        case RunMode_Usb:
            userdataManager->SetRunMode(RunMode_Online);
            break;
        case RunMode_Online:
            userdataManager->SetRunMode(RunMode_Local);
            break;
        case RunMode_Local:
            userdataManager->SetRunMode(RunMode_Usb);
            break;
        default:
            break;
        }
        Serial.end();
        delay(1000);
        ESP.restart();
        return;
    }
}

FuncButton::FuncButton()
{    
    pinMode(FUNC_BTN, INPUT);
    pressedTime = 0;
    
    scanner.attach(1.0, Scan);
}
