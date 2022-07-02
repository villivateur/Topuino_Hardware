#include <Arduino.h>
#include "DisplayPanel.h"
#include "OnlineConnector.h"
#include "UsbConnector.h"
#include "MonitorItems.h"
#include "StatusLed.h"
#include "WifiConfigManager.h"
#include "UserData.h"
#include "FuncButton.h"

DisplayPanel* displayPanel;
OnlineConnector* onlineConnector;
UsbConnector* usbConnector;

StatusLed* statusLed;
UserData* userdataManager;
WifiConfigManager* wifiConfigManager;
FuncButton* funcButton;

RunMode runMode;

void RunMode_Usb_Setup()
{
    usbConnector = new UsbConnector();
}

void RunMode_Online_Setup()
{
    if (!userdataManager->WifiInfoValid()) {
        wifiConfigManager = new WifiConfigManager();
        wifiConfigManager->ProcessConfig();
    } else {
        onlineConnector = new OnlineConnector();
    }
}

void RunMode_Local_Setup()
{
}

void setup()
{
    Serial.begin(115200);

    statusLed = new StatusLed();
    userdataManager = new UserData();
    displayPanel = new DisplayPanel();
    funcButton = new FuncButton();

    runMode = userdataManager->GetRunMode();

    switch (runMode) {
        case RunMode_Usb:
            displayPanel->DisplayNetRate(0, 0);
            RunMode_Usb_Setup();
            break;
        case RunMode_Online:
            displayPanel->DisplayNetRate(1024, 1024);
            RunMode_Online_Setup();
            break;
        case RunMode_Local:
            displayPanel->DisplayNetRate(2048, 2048);
            RunMode_Local_Setup();
            break;
        default:
            runMode = RunMode_Usb;
            displayPanel->DisplayNetRate(0, 0);
            userdataManager->SetRunMode(runMode);
            RunMode_Usb_Setup();
            break;
    }
}

void RunMode_Usb_Loop()
{
    displayPanel->DisplayCpuPercent(usbConnector->data.cpuPercent);
    displayPanel->DisplayMemPercent(usbConnector->data.memPercent);
    displayPanel->DisplayDisk0Percent(usbConnector->data.disk0Percent);
    displayPanel->DisplayDisk1Percent(usbConnector->data.disk1Percent);
    displayPanel->DisplayDiskRate(usbConnector->data.diskReadRate, usbConnector->data.diskWriteRate);
    displayPanel->DisplayNetRate(usbConnector->data.netSentRate, usbConnector->data.netReceiveRate);
}

void RunMode_Online_Loop()
{
    if (onlineConnector->FetchNewData() == OK) {
        displayPanel->DisplayCpuPercent(onlineConnector->GetPercent(CPU_PERCENT));
        displayPanel->DisplayMemPercent(onlineConnector->GetPercent(MEM_PERCENT));
        displayPanel->DisplayDisk0Percent(onlineConnector->GetPercent(DISK0_PERCENT));
        displayPanel->DisplayDisk1Percent(onlineConnector->GetPercent(DISK1_PERCENT));
        displayPanel->DisplayDiskRate(onlineConnector->GetRate(DISK_READ_RATE), onlineConnector->GetRate(DISK_WRITE_RATE));
        displayPanel->DisplayNetRate(onlineConnector->GetRate(NET_SENT_RATE), onlineConnector->GetRate(NET_RECV_RATE));
    } else {
        displayPanel->DisplayCpuPercent(0);
        displayPanel->DisplayMemPercent(0);
        displayPanel->DisplayDisk0Percent(0);
        displayPanel->DisplayDisk1Percent(0);
        displayPanel->DisplayDiskRate(0, 0);
        displayPanel->DisplayNetRate(0, 0);
    }
}

void RunMode_Local_Loop()
{
}

void loop()
{
    delay(1000);
    switch (runMode) {
        case RunMode_Usb:
            RunMode_Usb_Loop();
            break;
        case RunMode_Online:
            RunMode_Online_Loop();
            break;
        case RunMode_Local:
            RunMode_Local_Loop();
            break;
        default:
            break;
    }
}
