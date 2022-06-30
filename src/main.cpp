#include <Arduino.h>
#include "display.h"
#include "client_network.h"
#include "client_usb.h"
#include "monitor_items.h"
#include "status_blink.h"
#include "config_manager.h"
#include "user_data.h"
#include "func_button.h"

DisplayPanel* displayPanel;
ClientNetwork* netClient;
ClientUsb* usbClient;

StatusBlink* statusLed;
UserData* userdataManager;
ConfigManager* configManager;
FuncButton* funcButton;

RunMode runMode;

void RunMode_Usb_Setup()
{
    usbClient = new ClientUsb();
}

void RunMode_Online_Setup()
{
    if (!userdataManager->WifiInfoValid()) {
        configManager = new ConfigManager();
        configManager->ProcessConfig();
    } else {
        netClient = new ClientNetwork();
    }
}

void RunMode_Local_Setup()
{
}

void setup()
{
    Serial.begin(115200);

    statusLed = new StatusBlink();
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
    displayPanel->DisplayCpuPercent(usbClient->data.cpuPercent);
    displayPanel->DisplayMemPercent(usbClient->data.memPercent);
    displayPanel->DisplayDisk0Percent(usbClient->data.disk0Percent);
    displayPanel->DisplayDisk1Percent(usbClient->data.disk1Percent);
    displayPanel->DisplayDiskRate(usbClient->data.diskReadRate, usbClient->data.diskWriteRate);
    displayPanel->DisplayNetRate(usbClient->data.netSentRate, usbClient->data.netReceiveRate);
}

void RunMode_Online_Loop()
{
    if (netClient->FetchNewData() == OK) {
        displayPanel->DisplayCpuPercent(netClient->GetPercent(CPU_PERCENT));
        displayPanel->DisplayMemPercent(netClient->GetPercent(MEM_PERCENT));
        displayPanel->DisplayDisk0Percent(netClient->GetPercent(DISK0_PERCENT));
        displayPanel->DisplayDisk1Percent(netClient->GetPercent(DISK1_PERCENT));
        displayPanel->DisplayDiskRate(netClient->GetRate(DISK_READ_RATE), netClient->GetRate(DISK_WRITE_RATE));
        displayPanel->DisplayNetRate(netClient->GetRate(NET_SENT_RATE), netClient->GetRate(NET_RECV_RATE));
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
