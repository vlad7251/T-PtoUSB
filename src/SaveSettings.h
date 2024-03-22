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
    } settings;

    

    void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);

    void saveSettings();

    void loadSettings();

    void handleRoot(AsyncWebServerRequest *request);

    void handleSaveSettings(AsyncWebServerRequest *request);
};

#endif
