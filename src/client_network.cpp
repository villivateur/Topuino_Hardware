#include "client_network.h"

void ClientNetwork::Init()
{
    WiFi.begin("VVAILL", "channy161021");

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

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
