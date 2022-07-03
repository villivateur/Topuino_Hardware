#include "LocalConnector.h"
#include "StatusLed.h"
#include "UserData.h"
#include "DisplayPanel.h"

extern DisplayPanel* displayPanel;
extern UserData* userdataManager;
extern StatusLed* statusLed;

static byte packRecv[256];

LocalConnector::LocalConnector()
{
    WiFi.begin(userdataManager->GetWifiSsid(), userdataManager->GetWifiPasswd());

    statusLed->SetBlinkRate(StatusLed::BlinkRate::Rate2Hz);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
    }
    statusLed->SetBlinkRate(StatusLed::BlinkRate::RateAlwaysOff);

    displayPanel->DisplayDiskRate((uint32_t)(WiFi.localIP()[0]) * 1024, (uint32_t)(WiFi.localIP()[1]) * 1024);
    displayPanel->DisplayNetRate((uint32_t)(WiFi.localIP()[2]) * 1024, (uint32_t)(WiFi.localIP()[3]) * 1024);

    delay(2000);

    data = (UdpData*)(packRecv + 4);
    uint32_t port = 32737;
    Udp.begin(port);
}

void LocalConnector::FetchNewData()
{
    int packetSize = Udp.parsePacket();
    if (packetSize) {
        int len = Udp.read(packRecv, 255);
        byte packSend[2];

        if (len <= 0) {
            return;
        }
        if (packRecv[0] == 0x19 && packRecv[1] == 0x26 && packRecv[2] == 0x08 && packRecv[3] == 0x17) {
            packSend[0] = 0x00;
            packSend[1] = 0x00;
        } else {
            packSend[0] = 0x11;
            packSend[1] = 0x22;
            memset(packRecv, 0, sizeof(packRecv));
        }

        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        Udp.write(packSend, 2);
        Udp.endPacket();
    }
}
