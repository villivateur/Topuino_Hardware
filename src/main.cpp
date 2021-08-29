#include <Arduino.h>
#include "display.h"
#include "client_network.h"
#include "monitor_items.h"

DisplayPanel* displayPanel;
ClientNetwork* netClient;

void setup()
{
    Serial.begin(115200);
    Serial.println();

    displayPanel = new DisplayPanel();
    netClient = new ClientNetwork();
    netClient->Init();
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
}
