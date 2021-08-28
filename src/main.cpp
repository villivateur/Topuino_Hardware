#include <Arduino.h>
#include <TM1638plus.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

#define STROBE_TM1 13
#define STROBE_TM2 12 // strobe = GPIO connected to strobe line of module
#define STROBE_TM3 14
#define CLOCK_TM 4      // clock = GPIO connected to clock line of module
#define DIO_TM 5        // data = GPIO connected to data line of module
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true.

StaticJsonDocument<512> receivedData;

// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm1(STROBE_TM1, CLOCK_TM, DIO_TM, high_freq);
TM1638plus tm2(STROBE_TM2, CLOCK_TM, DIO_TM, high_freq);
TM1638plus tm3(STROBE_TM3, CLOCK_TM, DIO_TM, high_freq);

void setup()
{
    tm1.displayBegin();
    tm2.displayBegin();
    tm3.displayBegin();
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



void DisplayCpuPercent(uint8_t percent)
{
    uint16_t bitmap = PercentToBitmap(percent);
    tm1.display7Seg(0, bitmap % 0x100);
    tm1.display7Seg(1, bitmap / 0x100);
}

void DisplayMemPercent(uint8_t percent)
{
    uint16_t bitmap = PercentToBitmap(percent);
    tm1.display7Seg(2, bitmap % 0x100);
    tm1.display7Seg(3, bitmap / 0x100);
}

void DisplayDisk0Percent(uint8_t percent)
{
    uint16_t bitmap = PercentToBitmap(percent);
    tm1.display7Seg(4, bitmap % 0x100);
    tm1.display7Seg(5, bitmap / 0x100);
}

void DisplayDisk1Percent(uint8_t percent)
{
    uint16_t bitmap = PercentToBitmap(percent);
    tm1.display7Seg(6, bitmap % 0x100);
    tm1.display7Seg(7, bitmap / 0x100);
}

void DisplayDiskRate(uint32_t byteRdPerSec, uint32_t byteWrPerSec)
{
    uint16_t displayRd;
    uint16_t displayWr;
    char displayStr[9];

    if (byteRdPerSec < 1024 * 1024) {
        displayRd = byteRdPerSec / 1024;
        tm2.setLED(0, 0);
        tm2.setLED(1, 1);
    } else {
        displayRd = byteRdPerSec / 1024 / 1024;
        tm2.setLED(0, 1);
        tm2.setLED(1, 0);
    }
    if (byteWrPerSec < 1024 * 1024) {
        displayWr = byteWrPerSec / 1024;
        tm2.setLED(2, 0);
        tm2.setLED(3, 1);
    } else {
        displayWr = byteWrPerSec / 1024 / 1024;
        tm2.setLED(2, 1);
        tm2.setLED(3, 0);
    }
    sprintf(displayStr, "%4u%4u", displayRd, displayWr);
    tm2.displayText(displayStr);
}

void DisplayNetRate(uint32_t byteTxPerSec, uint32_t byteRxPerSec)
{
    uint16_t displayTx;
    uint16_t displayRx;
    char displayStr[9];

    if (byteTxPerSec < 1024 * 1024) {
        displayTx = byteTxPerSec / 1024;
        tm3.setLED(0, 0);
        tm3.setLED(1, 1);
    } else {
        displayTx = byteTxPerSec / 1024 / 1024;
        tm3.setLED(0, 1);
        tm3.setLED(1, 0);
    }
    if (byteRxPerSec < 1024 * 1024) {
        displayRx = byteRxPerSec / 1024;
        tm3.setLED(2, 0);
        tm3.setLED(3, 1);
    } else {
        displayRx = byteRxPerSec / 1024 / 1024;
        tm3.setLED(2, 1);
        tm3.setLED(3, 0);
    }
    sprintf(displayStr, "%4u%4u", displayTx, displayRx);
    tm3.displayText(displayStr);
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
                    DisplayCpuPercent(receivedData["CPU_PERCENT"]);
                    DisplayMemPercent(receivedData["MEM_PERCENT"]);
                    DisplayDisk0Percent(receivedData["DISK_PERCENT"]);
                    DisplayDisk1Percent(receivedData["DISK1_PERCENT"]);
                    DisplayDiskRate(receivedData["DISK_READ_RATE"], receivedData["DISK_WRITE_RATE"]);
                    DisplayNetRate(receivedData["NET_SENT_RATE"], receivedData["NET_RECV_RATE"]);
                }
            }
            http.end();
        }
    }
    delay(1000);
}
