#include "UserData.h"
#include <EEPROM.h>

#define MAX_USER_DATA_SIZE 0x200

#define WIFI_INFO_VALID_MAGIC 0xaa

#define WIFI_INFO_VALID_OFFSET 0x00
#define WIFI_SSID_VALID 0x20
#define WIFI_PASSWORD_OFFSET 0x40

#define RUN_MODE_OFFSET 0x60

UserData::UserData()
{
    EEPROM.begin(MAX_USER_DATA_SIZE);
}

String UserData::ReadEepromString(uint32_t offset)
{
    String result;
    char readByte;

    for (uint32_t addr = offset; (readByte = EEPROM.read(addr)) != '\0'; addr++) {
        result += readByte;
    }
    return result;
}

void UserData::WriteEepromString(String data, uint32_t offset)
{
    for (uint32_t i = 0; i < data.length(); i++) {
        EEPROM.write(i + offset, data[i]);
    }
    EEPROM.write(data.length() + offset, 0x00);
}

void UserData::EraseAllData()
{
    EEPROM.write(WIFI_INFO_VALID_OFFSET, 0x00);
    EEPROM.commit();
}

RunMode UserData::GetRunMode()
{
    mode = EEPROM.read(RUN_MODE_OFFSET);
    return (RunMode)mode;
}

void UserData::SetRunMode(RunMode mode)
{
    EEPROM.write(RUN_MODE_OFFSET, (byte)mode);
    EEPROM.commit();
}

bool UserData::WifiInfoValid()
{
    byte validation = EEPROM.read(WIFI_INFO_VALID_OFFSET);
    return validation == WIFI_INFO_VALID_MAGIC;
}

void UserData::ConfirmWifiData()
{
    EEPROM.write(WIFI_INFO_VALID_OFFSET, WIFI_INFO_VALID_MAGIC);
    EEPROM.commit();
}

String UserData::GetWifiSsid()
{
    return ReadEepromString(WIFI_SSID_VALID);
}

void UserData::SetWifiSsid(String ssid)
{
    return WriteEepromString(ssid, WIFI_SSID_VALID);
}

String UserData::GetWifiPasswd()
{
    return ReadEepromString(WIFI_PASSWORD_OFFSET);
}

void UserData::SetWifiPasswd(String password)
{
    return WriteEepromString(password, WIFI_PASSWORD_OFFSET);
}
