#include "WiFiSetup.h"

WiFiSetup::WiFiSetup(SaveSettings& settings) : saveSettings(settings) {}

void WiFiSetup::connectToWiFi() {
    Serial.write("Connecting to SSID: ");
    Serial.println(saveSettings.getStaSSID());

    WiFi.begin(saveSettings.getStaSSID(), saveSettings.getStaPassword());
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Settings loaded and WiFi connected.");

    WiFi.onEvent(std::bind(&WiFiSetup::WiFiGotIP, this, std::placeholders::_1, std::placeholders::_2), WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
}

void WiFiSetup::startSoftAP() {
   
}

void WiFiSetup::WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.write(("\n" + WiFi.localIP().toString() + "\n").c_str());
}
