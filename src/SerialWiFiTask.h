#ifndef SERIALWIFITASK_H
#define SERIALWIFITASK_H

#include "00_shared.h"
const int LED = 25;




class SerialWifiTask : public ITask {
public:
    SerialWifiTask();
    virtual bool Init() override;
    void DoTask() override;

private:
    void readSerial();
    void readWiFiToSerial();
    TickType_t prevCycleTime_us;
    TickType_t prevRecieveTime_us = 0;
    byte BufferWifi[1024];
    byte BufferSerial[1024];
    int bytesRead =0;
    int ledLeftMicros_us =0;
};

#endif  // SERIALWIFITASK_H
