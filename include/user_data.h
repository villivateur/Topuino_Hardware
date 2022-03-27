#include <Arduino.h>

class UserData
{
private:
    String ReadEepromString(uint32_t offset);
    void WriteEepromString(String data, uint32_t offset);
public:
    UserData();
    bool UserDataValid();
    void ConfirmData();
    void EraseData();
    String GetWifiSsid();
    void SetWifiSsid(String ssid);
    String GetWifiPasswd();
    void SetWifiPasswd(String password);
};
