#include "OnlineConnector.h"
#include "StatusLed.h"
#include "UserData.h"
#include "FuncButton.h"
#include "FactoryInfo.h"

extern StatusLed* statusLed;
extern UserData* userdataManager;
extern FuncButton* funcButton;

OnlineConnector::OnlineConnector()
{    
    WiFi.begin(userdataManager->GetWifiSsid(), userdataManager->GetWifiPasswd());

    statusLed->SetBlinkRate(StatusLed::BlinkRate::Rate2Hz);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
    }
    statusLed->SetBlinkRate(StatusLed::BlinkRate::RateAlwaysOff);
    url = "http://iot.vvzero.com/topuino/getdata?SN=";
    url += DEVICE_SN;
    status = FAIL;
}

STATUS OnlineConnector::FetchNewData()
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

uint8_t OnlineConnector::GetPercent(String name)
{
    return status == OK ? receivedData[name] : 0;
}

uint32_t OnlineConnector::GetRate(String name)
{
    return status == OK ? receivedData[name] : 0;
}
