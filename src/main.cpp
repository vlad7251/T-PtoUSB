#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <Wire.h>
#include <SerialWiFiTask.h>
#include <00_shared.h>
#include <SaveSettings.h>
#include <WebServer.h>

const char* ssid = "HONOR 9C";  
const char* password = "vlad7251"; 

SerialWifiTask g_SerialWifiTask;

//SaveSettings g_saveSettings;


void setup() {
  Serial.setTxBufferSize(1024);
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(1000);
  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);
  /*WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }*/
  //g_saveSettings.begin();
  g_WebServer.begin();
  g_SerialWifiTask.Init();

  
}

void loop() {

  
}
  
  



      // Чтение из Serial и отправка клиенту
      //TODO Зажтигать светодиод во время передачи
      //TODO обычный serial - 0, c заданными pin TX - GPIO17, RX - GPIO16, RO - GPIO5
     /*while (true) {
        int size = Serial.available();
        if(!size) 
          break;
        if(size>sizeof(buffer))
          size = sizeof(buffer);
        bytesRead = Serial.readBytes(buffer, size);
        digitalWrite(LED, HIGH);
        client.write(buffer, bytesRead);
        digitalWrite(LED, LOW);
      }*/