#include "client_network.h"
#include "status_blink.h"
#include "user_data.h"
#include "func_button.h"
#include "factory_info.h"

extern StatusBlink* statusLed;
extern UserData* userdataManager;
extern FuncButton* funcButton;

ClientNetwork::ClientNetwork()
{    
    WiFi.begin(userdataManager->GetWifiSsid(), userdataManager->GetWifiPasswd());

    statusLed->SetBlinkRate(StatusBlink::BlinkRate::Rate2Hz);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        funcButton->Scan();
    }
    statusLed->SetBlinkRate(StatusBlink::BlinkRate::RateAlwaysOff);
    url = "http://iot.vvzero.com/topuino/getdata?SN=";
    url += DEVICE_SN;
    status = FAIL;
}

STATUS ClientNetwork::FetchNewData()
{
    status = FAIL;
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        http = new HTTPClient();
        if (http->begin(client, url)) {
            if (http->GET() == HTTP_CODE_OK) {
                if (deserializeJson(receivedData, http->getString().c_str()) == DeserializationError::Code::Ok) {
                    status = OK;
                }
            }
            http->end();
        }
        delete http;
    }
    return status;
}

uint8_t ClientNetwork::GetPercent(String name)
{
    return status == OK ? receivedData[name] : 0;
}

uint32_t ClientNetwork::GetRate(String name)
{
    return status == OK ? receivedData[name] : 0;
}
