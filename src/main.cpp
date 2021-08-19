#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

IPAddress local_IP(192,168,4,1);
IPAddress gateway(192,168,4,1);
IPAddress subnet(255,255,255,0);

Ticker displayTicker;
const uint8_t g_displayFlushGpio[3] = {13, 12, 14};
uint16_t g_displayData[3] = {0x01ff, 0x007f, 0x7fff};

StaticJsonDocument<512> receivedData;

void displayFlush()
{
    static uint8_t displayLine = 0;

    digitalWrite(g_displayFlushGpio[displayLine], LOW);
    displayLine++;
    if (displayLine == 3) displayLine = 0;
    Wire.beginTransmission(0x20);
    Wire.write(0x02);
    Wire.write((uint8_t)(g_displayData[displayLine] % 0x100));
    Wire.write((uint8_t)(g_displayData[displayLine] / 0x100));
    Wire.endTransmission();
    digitalWrite(g_displayFlushGpio[displayLine], HIGH);
}

void setup()
{
    Serial.begin(115200);
    Serial.println();

    pinMode(13, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(14, OUTPUT);
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(14, LOW);

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

    Wire.begin(4, 5);

    Wire.beginTransmission(0x20);
    Wire.write(0x06);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.beginTransmission(0x20);
    Wire.write(0x02);
    Wire.write(0xff);
    Wire.write(0xff);
    Wire.endTransmission();

    displayTicker.attach(0.005, displayFlush);
}

uint16_t PercentToBitmap(uint8_t percent)
{
    uint32_t tempPercent16;
    uint16_t tempDisplayBit;

    tempPercent16 = percent * 15 / 100;
    tempDisplayBit = 0x1;
    for (uint8_t i = 0; i < tempPercent16; i++) {
        tempDisplayBit <<= 1;
        tempDisplayBit += 1;
    }

    return tempDisplayBit;
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;

        if (http.begin(client, "http://iot.vvzero.com/topuino/getdata?UUID=1e788f28-7a5e-4888-96ff-71ab8b1876f8")) {
            int httpCode = http.GET();
            if (httpCode == HTTP_CODE_OK) {
                DeserializationError jsonError = deserializeJson(receivedData, http.getString().c_str());
                if (jsonError) {
                    Serial.println(http.getString().c_str());
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(jsonError.f_str());
                } else {
                    g_displayData[0] = PercentToBitmap(receivedData["CPU_PERCENT"]);
                    g_displayData[1] = PercentToBitmap(receivedData["MEM_PERCENT"]);
                    g_displayData[2] = PercentToBitmap(receivedData["DISK_PERCENT"]);
                }
            }
            http.end();
        }
    }
    delay(1000);
}