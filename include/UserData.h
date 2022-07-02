#include <Arduino.h>

enum RunMode
{
    RunMode_Start = 0x20,

    RunMode_Usb = RunMode_Start,
    RunMode_Online,
    RunMode_Local,

    RunMode_End,
};

class UserData
{
private:
    String ReadEepromString(uint32_t offset);
    void WriteEepromString(String data, uint32_t offset);
    byte mode;

public:
    UserData();
    void EraseAllData();

    RunMode GetRunMode();
    void SetRunMode(RunMode mode);
    void SwitchRunMode();

    bool WifiInfoValid();
    void ConfirmWifiData();
    String GetWifiSsid();
    void SetWifiSsid(String ssid);
    String GetWifiPasswd();
    void SetWifiPasswd(String password);
};
