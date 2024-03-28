#ifndef SaveSettings_h
#define SaveSettings_h
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include <nvs_flash.h>
#include <nvs.h>



class SaveSettings {
public:


    void begin();

private:
    Preferences preferences;   
    struct Settings {
        char staSSID[32];
        char staPassword[32];
        int baudRate;
        short transparentPort;
        short commandPort;
        bool operator==(const Settings& other) const {
        return strcmp(staSSID, other.staSSID) == 0 &&
               strcmp(staPassword, other.staPassword) == 0 &&
               baudRate == other.baudRate &&
               transparentPort == other.transparentPort &&
               commandPort == other.commandPort;
        }
    };
    Settings settings;

    /*bool operator==(const Settings& lhs, const Settings& rhs) {
        return strcmp(lhs.staSSID, rhs.staSSID) == 0 &&
         strcmp(lhs.staPassword, rhs.staPassword) == 0 &&
         lhs.baudRate == rhs.baudRate &&
         lhs.transparentPort == rhs.transparentPort &&
         lhs.commandPort == rhs.commandPort;
    }*/

    

    void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);

    void saveSettings();

    void loadSettings();

    void handleRoot(AsyncWebServerRequest *request);

    void handleSaveSettings(AsyncWebServerRequest *request);
};

#endif
