#include "config_manager.h"
#include "status_blink.h"
#include "user_data.h"

#define SSID "Topuino"
#define WIFI_PASSWORD "vvzero.com"

extern StatusBlink* statusLed;
extern ConfigManager* configManager;
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
    configManager->httpServer.send(200, "text/html", configPage);
}

static void handleOnCommit()
{
    if (configManager->httpServer.hasArg("ssid") && configManager->httpServer.hasArg("psw")) {
        userdataManager->SetWifiSsid(configManager->httpServer.arg("ssid"));
        userdataManager->SetWifiPasswd(configManager->httpServer.arg("psw"));
        userdataManager->ConfirmWifiData();
        configManager->httpServer.send(200, "text/html", "OK");
        delay(200);
        ESP.restart();
    } else {
        configManager->httpServer.send(200, "text/html", "ERROR");
    }
}

ConfigManager::ConfigManager() :
localIp(192,168,1,1),
gateway(192,168,1,1),
subnet(255,255,255,0),
httpServer(80)
{
    statusLed->SetBlinkRate(StatusBlink::BlinkRate::Rate0_5Hz);
    WiFi.softAPConfig(localIp, gateway, subnet);
    WiFi.softAP(SSID, WIFI_PASSWORD);
    delay(100);
    
    httpServer.on("/", handleOnConnect);
    httpServer.on("/setup", handleOnCommit);
    
    httpServer.begin();
}

void ConfigManager::ProcessConfig()
{
    while (1) {
        httpServer.handleClient();
    }
}
