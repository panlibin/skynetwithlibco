#include "../common/common.h"
#include "monitor.h"
#include "timer.h"
#include "servicemanager.h"
#include "timerthread.h"

using namespace csn;

TimerThread::TimerThread(Monitor* pMonitor)
    : m_pMonitor(pMonitor)
{
    
}

void TimerThread::run()
{
    for (;;)
    {
        g_Timer.updateTime();
        if (g_ServiceManager.getServiceCount() == 0) break;
        m_pMonitor->wakeup(m_pMonitor->getThreadCount() - 1);
        usleep(2500);
    }
    m_pMonitor->quit();
}
