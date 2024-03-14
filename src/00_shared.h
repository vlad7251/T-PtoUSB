#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
using namespace std;

#define OUT
#define IN
#define countof(a) (sizeof(a)/sizeof((a)[0]))

extern const char* g_sProgramName; // идентификатор типа прошивки для хранения в EEPROM(Preferences.h)
extern const uint16_t g_uVersion; // версия прошивки

class ITask
{
private:
    friend void ITask_DoTask(ITask* thisClass);
    // увеличить счётчик активных задач,
    // вернёт false, если задаче не разрешено запускаться из-за активной другой ресурсотребовательной задачи. В этом случае счётчик не инкрементируется.
    static bool ActiveTaskCounterIncrement();
    // Отметить, что текущаяы задача перешла в режим ожидания
    static void ActiveTaskCounterDecrement();
protected:
    TickType_t m_xCallTimeShift_ms; // вызвать задачу в следующий раз через столько ms
    bool InitInternal(TickType_t xFrequency, const char* taskName, uint32_t stackSize = 2048);
    virtual void DoTask() = 0;
    // Заблокировать запуск других задач, для случая, когда текущая задача требует монопольного использования процессора.
    // Разлочка осуществляется через UnLockActiveTaskCounter.
    // Если есть другие активные задачи(кроме текущей), то блокировки не произойдёт(как и инкремента счётчика активных задач),
    // И будет возвращено false.
    static bool LockActiveTaskCounter();
    // Разблокировать запуск других задач.
    static void UnLockActiveTaskCounter();
public:
    virtual bool Init() = 0;
};
void GetAllTasksList(OUT TaskHandle_t** pFirst, OUT TaskHandle_t** pLast);

// #define ENABLE_WEBSEVER
// #define ENABLE_PWM_CONTROL
