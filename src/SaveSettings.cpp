#include "SaveSettings.h"

const char *settingsKey = "TCPtoRS485";

void SaveSettings::begin() 
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(err = nvs_flash_erase());
        ESP_ERROR_CHECK(err = nvs_flash_init());
        Serial.write("\nWarning! NVS erased");
    }
    bool flash_connection = false;
    if (!preferences.begin(settingsKey, false)) {
        Serial.println("Failed to open Preferences");
        flash_connection = false;
    } else {
        loadSettings();
        flash_connection = true;
    }

    if (flash_connection == false || (settings.staSSID) == 0 || strlen(settings.staPassword) == 0) {
        Serial.println("Default settings used");
        strcpy(settings.staSSID, "HONOR 9C");
        saveSettings("staSSID");
        strcpy(settings.staPassword, "vlad7251");
        saveSettings("staPassword");
        settings.baudRate = 115200;
        saveSettings("baudRate");
        Serial.updateBaudRate(settings.baudRate);
        settings.transparentPort = 8080;
        saveSettings("transparentPort");
        settings.commandPort = 1234;
        saveSettings("commandPort");
    }

    
   
}

void SaveSettings::WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) 
{
    Serial.write(("\n" + WiFi.localIP().toString() + "\n").c_str());
}

void SaveSettings::loadSettings() 
{
    preferences.getBytes(settingsKey, &settings, sizeof(Settings));
}

void SaveSettings::saveSettings(const char* fieldName) {
    if (strcmp(fieldName, "staSSID") == 0) {
        preferences.putBytes(settingsKey, &settings.staSSID, sizeof(settings.staSSID));
    } else if (strcmp(fieldName, "staPassword") == 0) {
        preferences.putBytes(settingsKey, &settings.staPassword, sizeof(settings.staPassword));
    } else if (strcmp(fieldName, "baudRate") == 0) {
        preferences.putBytes(settingsKey, &settings.baudRate, sizeof(settings.baudRate));
    } else if (strcmp(fieldName, "transparentPort") == 0) {
        preferences.putBytes(settingsKey, &settings.transparentPort, sizeof(settings.transparentPort));
    } else if (strcmp(fieldName, "commandPort") == 0) {
        preferences.putBytes(settingsKey, &settings.commandPort, sizeof(settings.commandPort));
    }
}

// Геттеры и сеттеры для каждого поля settings

const char* SaveSettings::getStaSSID() const 
{
    return settings.staSSID;
}

void SaveSettings::setStaSSID(const char* ssid)
{
    strncpy(settings.staSSID, ssid, sizeof(settings.staSSID));
    saveSettings("staSSID");
}

const char* SaveSettings::getStaPassword() const
{
    return settings.staPassword;
}

void SaveSettings::setStaPassword(const char* password)
{
    strncpy(settings.staPassword, password, sizeof(settings.staPassword));
    saveSettings("staPassword");
}

int SaveSettings::getBaudRate() const
{
    return settings.baudRate;
}

void SaveSettings::setBaudRate(int baudRate) 
{
    settings.baudRate = baudRate;
    Serial.updateBaudRate(settings.baudRate);
    saveSettings("baudRate");
}

int SaveSettings::getTransparentPort() const 
{
    return settings.transparentPort;
}

void SaveSettings::setTransparentPort(int transparentPort) 
{
    settings.transparentPort = transparentPort;
    saveSettings("transparentPort");
}

int SaveSettings::getCommandPort() const 
{
    return settings.commandPort;
}

void SaveSettings::setCommandPort(int commandPort) 
{
    settings.commandPort = commandPort;
    saveSettings("commandPort");
}
