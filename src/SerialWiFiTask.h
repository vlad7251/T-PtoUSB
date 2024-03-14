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
    byte BufferWifi[1024];
    byte BufferSerial[1024];
    bool packetReceived;
    int bytesRead;
    int ledLeftMicros_us;
};

#endif  // SERIALWIFITASK_H
