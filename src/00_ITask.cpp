#include "00_shared.h"

uint32_t g_nCurTasks = 0;
TaskHandle_t g_vTasks[21] = {nullptr};

void GetAllTasksList(OUT TaskHandle_t** pFirst, OUT TaskHandle_t** pLast)
{
    *pFirst =  g_vTasks;
    *pLast = g_vTasks + g_nCurTasks;
}

void ITask_DoTask(ITask* thisClass)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    while(true)
    {
        TickType_t callTimeShift_ms;
        if (ITask::ActiveTaskCounterIncrement())
        {
            thisClass->DoTask();
            ITask::ActiveTaskCounterDecrement();
            callTimeShift_ms = thisClass->m_xCallTimeShift_ms;
        }
        else
            callTimeShift_ms = 0;
        if (callTimeShift_ms <= 0)
        {
            taskYIELD();
            xLastWakeTime = xTaskGetTickCount(); // на случай дальнейшего переключения интервала шагания
        }
        else
        {
            BaseType_t xWasDelayed = xTaskDelayUntil( IN OUT &xLastWakeTime, callTimeShift_ms );
            // xWasDelayed - будет true если выполнение задачи заняло так много времени, что заняло весь квант и более
        }
    }
}

bool ITask::InitInternal(TickType_t xFrequency, const char* taskName, uint32_t stackSize /*= 2048*/)
{
    m_xCallTimeShift_ms = xFrequency;
    int app_cpu = xPortGetCoreID();
    TaskHandle_t h1;
    xTaskCreatePinnedToCore(reinterpret_cast<TaskFunction_t>(&ITask_DoTask), taskName, stackSize, this, 1, OUT g_vTasks + g_nCurTasks, app_cpu);
    assert(g_vTasks[g_nCurTasks] != nullptr);
    if (g_vTasks[g_nCurTasks] == nullptr)
        return false;
    g_nCurTasks++;
    assert(g_nCurTasks < countof(g_vTasks));
    return true;
}

volatile int g_nActiveTaskCounter = 0;
volatile int g_nTasksLocksCnt = 0;
// увеличить счётчик активных задач,
// вернёт false, если задаче не разрешено запускаться из-за активной другой ресурсотребовательной задачи. В этом случае счётчик не инкрементируется.
/*static*/ bool ITask::ActiveTaskCounterIncrement()
{
    if (g_nTasksLocksCnt)
        return false;
    g_nActiveTaskCounter++;
    return true;
}
// Отметить, что текущаяы задача перешла в режим ожидания
/*static*/ void ITask::ActiveTaskCounterDecrement()
{
    if (g_nActiveTaskCounter > 0)
        g_nActiveTaskCounter--;
}
// Заблокировать запуск других задач, для случая, когда текущая задача требует монопольного использования процессора.
// Разлочка осуществляется через UnLockActiveTaskCounter.
// Если есть другие активные задачи(кроме текущей), то блокировки не произойдёт(как и инкремента счётчика активных задач),
// И будет возвращено false.
/*static*/ bool ITask::LockActiveTaskCounter()
{
    if (g_nTasksLocksCnt)
        return false;
    if (g_nActiveTaskCounter != 1)    
        return false;
    g_nTasksLocksCnt++;
    return true;
}
// Разблокировать запуск других задач.
/*static*/ void ITask::UnLockActiveTaskCounter()
{
    if (g_nTasksLocksCnt > 0)
        g_nTasksLocksCnt--;
}
