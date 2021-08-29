#include <Arduino.h>
#include "display.h"
#include "client_network.h"
#include "monitor_items.h"
#include "status_blink.h"

DisplayPanel* displayPanel;
ClientNetwork* netClient;
StatusBlink* statusLed;

void setup()
{
    statusLed = new StatusBlink();
    displayPanel = new DisplayPanel();
    netClient = new ClientNetwork();
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
