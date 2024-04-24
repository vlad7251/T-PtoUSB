#ifndef SAVE_SETTINGS_H
#define SAVE_SETTINGS_H

#include <Arduino.h>
#include <Preferences.h>
#include <WiFi.h>
#include <nvs_flash.h>
#include <nvs.h>

struct Settings {
    char staSSID[32];
    char staPassword[64];
    int baudRate;
    int transparentPort;
    int commandPort;
    bool operator==(const Settings& other) const {
        return strcmp(staSSID, other.staSSID) == 0 &&
               strcmp(staPassword, other.staPassword) == 0 &&
               baudRate == other.baudRate &&
               transparentPort == other.transparentPort &&
               commandPort == other.commandPort;
        }
};

class SaveSettings {
public:
    void begin();
    const char* getStaSSID() const;
    void setStaSSID(const char* ssid);
    const char* getStaPassword() const;
    void setStaPassword(const char* password);
    int getBaudRate() const;
    void setBaudRate(int baudRate);
    int getTransparentPort() const;
    void setTransparentPort(int transparentPort);
    int getCommandPort() const;
    void setCommandPort(int commandPort);

private:
    Settings settings;
    Preferences preferences;
    const char *settingsKey = "TCPtoRS485";

    void loadSettings();
    void saveSettings(const char* fieldName);
    void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);
};

#endif
