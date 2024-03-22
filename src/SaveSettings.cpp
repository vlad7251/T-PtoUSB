#include "SaveSettings.h"
const char *settingsKey = "TCPtoRS485";
AsyncWebServer server(2000);

bool operator==(const Settings& lhs, const Settings& rhs) {
  return strcmp(lhs.staSSID, rhs.staSSID) == 0 &&
         strcmp(lhs.staPassword, rhs.staPassword) == 0 &&
         lhs.baudRate == rhs.baudRate &&
         lhs.transparentPort == rhs.transparentPort &&
         lhs.commandPort == rhs.commandPort;
}

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
        pinMode(LED, OUTPUT);
        delay(300);
        digitalWrite(LED, HIGH);
        delay(300);
        digitalWrite(LED, LOW);
        Serial.println("Failed to open Preferences");
        flash_connection = false;
    } else {
        loadSettings();
        flash_connection = true;
    }

    if (flash_connection == false || (settings.staSSID) == 0 || strlen(settings.staPassword) == 0) {
        Serial.println("Default settings used");
        strcpy(settings.staSSID, "HONOR 9C");
        strcpy(settings.staPassword, "vlad7251");
        settings.baudRate = 115200;
        settings.transparentPort = 8080;
        settings.commandPort = 1234;
        saveSettings();
    }

    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    Serial.write("Connecting to SSID: ");
    Serial.println(settings.staSSID);

    WiFi.begin(settings.staSSID, settings.staPassword);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        handleRoot(request);
    });

    server.on("/save", HTTP_POST, [](AsyncWebServerRequest *request){
        handleSaveSettings(request);
    });

    server.begin();
}

void SaveSettings::WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.write(("\n" + WiFi.localIP().toString() + "\n").c_str());
}

void SaveSettings::loadSettings() {
    preferences.getBytes(settingsKey, &settings, sizeof(Settings));
}

void SaveSettings::handleRoot(AsyncWebServerRequest *request) {
    String html = R"html(
        <html>
            <head>
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        margin: 20px;
                    }
                    h1 {
                        color: #333333;
                    }
                    form {
                        width: 300px;
                        margin: 20px 0;
                    }
                    label {
                        display: block;
                        margin-bottom: 5px;
                    }
                    input {
                        width: 100%;
                        padding: 8px;
                        margin-bottom: 10px;
                    }
                    input[type="submit"] {
                        background-color: #4CAF50;
                        color: white;
                        cursor: pointer;
                    }
                </style>
            </head>
            <body>
                <h1>WiFi Settings</h1>
                <form action='/save' method='post'>
                    <label for='ssid'>SSID:</label>
                    <input type='text' name='ssid' value='" + String(settings.staSSID) + "' required><br>
                    <label for='password'>Password:</label>
                    <input type='password' name='password' placeholder='Enter new password' required><br>
                    <label for='baud'>Baud Rate:</label>
                    <input type='number' name='baud' value='" + String(settings.baudRate) + "' required><br>
                    <label for='transPort'>Transparent Port:</label>
                    <input type='number' name='transPort' value='" + String(settings.transparentPort) + "' required><br>
                    <label for='commandPort'>Command Port:</label>
                    <input type='number' name='commandPort' value='" + String(settings.commandPort) + "' required><br>
                    <input type='submit' value='Save'>
                </form>
            </body>
        </html>
    )html";

    request->send(200, "text/html", html);
}

void SaveSettings::handleSaveSettings(AsyncWebServerRequest *request) {
   Settings newSettings = settings;


    if (request->hasParam("ssid", true))
        strncpy(newSettings.staSSID,  request->getParam("ssid", true)->value().c_str(), sizeof(newSettings.staSSID));
    if (request->hasParam("password", true))
        strncpy(newSettings.staPassword,  request->getParam("password", true)->value().c_str(), sizeof(newSettings.staPassword));
    if (request->hasParam("baud", true)) 
        newSettings.baudRate = request->getParam("baud", true)->value().toInt();
    if (request->hasParam("transPort", true)) 
        newSettings.transparentPort = request->getParam("transPort", true)->value().toInt();
    if (request->hasParam("commandPort", true))
        newSettings.commandPort = request->getParam("commandPort", true)->value().toInt();
    

    if (!(settings == newSettings)) {
        settings = newSettings;
        preferences.putBytes(settingsKey, &settings, sizeof(Settings));
        request->send(200, "text/plain", "Settings saved");
    } else {
        request->send(200, "text/plain", "Settings unchanged");
    }
}
