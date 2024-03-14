#include "SerialWifiTask.h"
#include <WiFi.h>
WiFiServer server;
WiFiClient client;

SerialWifiTask::SerialWifiTask()  
{
    prevCycleTime_us = micros();
}
bool SerialWifiTask::Init()
{
   server.begin(2001);
   return ITask::InitInternal(0, "SerialWifiTask", 8192);   
}

void SerialWifiTask::DoTask() {
    
    TickType_t currentTime_us = micros();
    int deltaMicros = currentTime_us-prevCycleTime_us;
    if (ledLeftMicros_us < deltaMicros)
        ledLeftMicros_us = 0;
    else
        ledLeftMicros_us -= deltaMicros;    
    int size_serial = Serial.available();
    if (size_serial > 0) {
        if (size_serial > sizeof(BufferSerial))
            size_serial = sizeof(BufferSerial);
        bytesRead = Serial.readBytes(BufferSerial, size_serial);
        packetReceived = true;
        //startTime_us = micros();
    }
    if (packetReceived && (deltaMicros >= 1000)) { //TODO неправильно записан таймаут в 32 строке
        client.write(BufferSerial, bytesRead);
        packetReceived = false;
    }
    readWiFiToSerial(); //TODO digitalWrite
    prevCycleTime_us = currentTime_us;
    digitalWrite(LED, ledLeftMicros_us > 0);
}
//TODO разобраться с проблемой: реализация сдвига в буфере, чтобы даннные не перезаписывались поверх принятых
//TODO добавить условие для отправки данных когда заполняется буфер
//TODO если пришла команда по wifi, то необходимо терять данные из приёмного буфера
//TODO провести тестирование в обе стороны

void SerialWifiTask::readWiFiToSerial() {
    
    WiFiClient client2 = server.available();
    if (client2)
    {
        Serial.write("reconnect\n");
        client.stop();
        client = std::move(client2);
    }
    static bool shown = false;
    if (!client) {
        if (!shown) {
            shown = true;
            Serial.write("disconnected\n");
        }
        return;
    }
    if(shown)
    {
        Serial.write("connected\n");
        shown = false;
    }
    int size = client.available();
    if (size > 0) 
    {
        if (size > sizeof(BufferWifi))
            size = sizeof(BufferWifi);
        bytesRead = client.readBytes(BufferWifi, size);
        //digitalWrite(5, HIGH);
        Serial.write(BufferWifi, bytesRead);
        //digitalWrite(5, LOW);     
        //TODO раскомментировать при переходе на 485 интерфейс        
    }
}

//TODO сделать время измерения полями класса, сделать измерение таймаута 1 мс после последнего полученного байта 
  //FIXME Поменять название проекта
