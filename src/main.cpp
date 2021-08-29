#include <Arduino.h>
#include "display.h"
#include "client_network.h"
#include "monitor_items.h"
#include "status_blink.h"
#include "config_manager.h"
#include "user_data.h"
#include "func_button.h"

DisplayPanel* displayPanel;
ClientNetwork* netClient;
StatusBlink* statusLed;
UserData* userdataManager;
ConfigManager* configManager;
FuncButton* funcButton;

void setup()
{
    statusLed = new StatusBlink();
    userdataManager = new UserData();
    displayPanel = new DisplayPanel();
    funcButton = new FuncButton();

    if (!userdataManager->UserDataValid()) {
        configManager = new ConfigManager();
        configManager->ProcessConfig();
    } else {
        netClient = new ClientNetwork();
    }
}

void loop()
{
    delay(1000);
    if (netClient->FetchNewData() != OK) {
        return;
    }
    displayPanel->DisplayCpuPercent(netClient->GetPercent(CPU_PERCENT));
    displayPanel->DisplayMemPercent(netClient->GetPercent(MEM_PERCENT));
    displayPanel->DisplayDisk0Percent(netClient->GetPercent(DISK0_PERCENT));
    displayPanel->DisplayDisk1Percent(netClient->GetPercent(DISK1_PERCENT));
    displayPanel->DisplayDiskRate(netClient->GetRate(DISK_READ_RATE), netClient->GetRate(DISK_WRITE_RATE));
    displayPanel->DisplayNetRate(netClient->GetRate(NET_SENT_RATE), netClient->GetRate(NET_RECV_RATE));

    funcButton->Scan();
}
