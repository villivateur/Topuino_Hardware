#include "WifiConfigManager.h"
#include "StatusLed.h"
#include "UserData.h"

#define SSID "Topuino"
#define WIFI_PASSWORD "vvzero.com"

extern StatusLed* statusLed;
extern WifiConfigManager* wifiConfigManager;
extern UserData* userdataManager;

static String configPage("\
<!DOCTYPE html>\
<html>\
<head>\
    <title>Topuino</title>\
    <meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no' />\
    <link rel='shortcut icon' href='data:,'>\
</head>\
<body>\
<h1 style='text-align: center; font-size: 40px'>Topuino Config Page</h1>\
<form action='/setup' method='POST'>\
    <div style='text-align: center'>\
        <div style='font-size: 20px; margin-bottom: 15px'>\
            <label for='ssid'><b>Wi-Fi SSID</b></label>\
            <input type='text' name='ssid' required>\
        </div>\
        <div style='font-size: 20px; margin-bottom: 15px'>\
            <label for='psw'><b>Wi-Fi Password</b></label>\
            <input type='password' name='psw' required>\
        </div>\
        <div style='font-size: 20px'>\
            <button type='submit'>Commit</button>\
        </div>\
    </div>\
</form>\
</body>\
</html>\
");


static void handleOnConnect()
{
    wifiConfigManager->httpServer.send(200, "text/html", configPage);
}

static void handleOnCommit()
{
    if (wifiConfigManager->httpServer.hasArg("ssid") && wifiConfigManager->httpServer.hasArg("psw")) {
        userdataManager->SetWifiSsid(wifiConfigManager->httpServer.arg("ssid"));
        userdataManager->SetWifiPasswd(wifiConfigManager->httpServer.arg("psw"));
        userdataManager->ConfirmWifiData();
        wifiConfigManager->httpServer.send(200, "text/html", "OK");
        statusLed->SetBlinkRate(StatusLed::BlinkRate::RateAlwaysOn);
        delay(200);
        ESP.restart();
    } else {
        wifiConfigManager->httpServer.send(200, "text/html", "ERROR");
    }
}

WifiConfigManager::WifiConfigManager() :
localIp(192,168,1,1),
gateway(192,168,1,1),
subnet(255,255,255,0),
httpServer(80)
{
    statusLed->SetBlinkRate(StatusLed::BlinkRate::Rate0_5Hz);
    WiFi.softAPConfig(localIp, gateway, subnet);
    WiFi.softAP(SSID, WIFI_PASSWORD);
    delay(100);
    
    httpServer.on("/", handleOnConnect);
    httpServer.on("/setup", handleOnCommit);
    
    httpServer.begin();
}

void WifiConfigManager::ProcessConfig()
{
    while (1) {
        httpServer.handleClient();
    }
}
