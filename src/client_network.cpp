#include "client_network.h"
#include "status_blink.h"

extern StatusBlink* statusLed;

ClientNetwork::ClientNetwork()
{
    WiFi.begin("VVAILL", "channy161021");

    statusLed->SetBlinkRate(StatusBlink::BlinkRate::Rate2Hz);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    statusLed->SetBlinkRate(StatusBlink::BlinkRate::RateAlwaysOff);

    status = FAIL;
}

STATUS ClientNetwork::FetchNewData()
{
    status = FAIL;
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        http = new HTTPClient();
        if (http->begin(client, "http://iot.vvzero.com/topuino/getdata?UUID=1e788f28-7a5e-4888-96ff-71ab8b1876f8")) {
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
