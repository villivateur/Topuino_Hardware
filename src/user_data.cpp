#include "user_data.h"
#include <EEPROM.h>

#define MAX_USER_DATA_SIZE 0x200

#define VALIDATION_MAGIC 0xaa

#define GENERAL_FEILD_SIZE 0x20

#define VALIDATION_OFFSET 0x00
#define SSID_OFFSET 0x20
#define PASSWORD_OFFSET 0x40

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

bool UserData::UserDataValid()
{
    byte validation = EEPROM.read(VALIDATION_OFFSET);
    return validation == VALIDATION_MAGIC;
}

void UserData::ConfirmData()
{
    EEPROM.write(VALIDATION_OFFSET, VALIDATION_MAGIC);
    EEPROM.commit();
}

void UserData::EraseData()
{
    EEPROM.write(VALIDATION_OFFSET, 0x00);
    EEPROM.commit();
}

String UserData::GetWifiSsid()
{
    return ReadEepromString(SSID_OFFSET);
}

void UserData::SetWifiSsid(String ssid)
{
    return WriteEepromString(ssid, SSID_OFFSET);
}

String UserData::GetWifiPasswd()
{
    return ReadEepromString(PASSWORD_OFFSET);
}

void UserData::SetWifiPasswd(String password)
{
    return WriteEepromString(password, PASSWORD_OFFSET);
}
