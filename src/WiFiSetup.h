#ifndef WIFI_SETUP_H
#define WIFI_SETUP_H

#include <WiFi.h>
#include "SaveSettings.h"

class WiFiSetup {
private:
    SaveSettings& saveSettings;

    void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
public:
    WiFiSetup(SaveSettings& settings);

    void connectToWiFi();
    void startSoftAP();
};

#endif 
