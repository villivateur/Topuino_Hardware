#include "UsbConnector.h"

static byte* dataBuff;

static void Receive()
{
    static byte raw[sizeof(UsbData) + 4];
    Serial.readBytes(raw, sizeof(UsbData) + 4);
    if (raw[0] == 0x19 && raw[1] == 0x26 && raw[2] == 0x08 && raw[3] == 0x17) {
        byte pongBuf[2] = { 0x68, 0x61 };
        Serial.write(pongBuf, 2);
    } else if (raw[0] == 0x66 && raw[1] == 0x77 && raw[2] == 0xaa && raw[3] == 0xff) {
        memcpy(dataBuff, raw + 4, sizeof(UsbData));
    }
}

UsbConnector::UsbConnector()
{
    memset(&data, 0, sizeof(UsbData));
    Serial.setTimeout(100);
    dataBuff = (byte*)&data;
    receiver.attach(0.5, Receive);
}
