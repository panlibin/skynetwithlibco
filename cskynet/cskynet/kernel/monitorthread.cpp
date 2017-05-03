#include "../common/common.h"
#include "monitor.h"
#include "monitorthread.h"
#include "servicemanager.h"

using namespace csn;

MonitorThread::MonitorThread(Monitor* pMonitor)
    : m_pMonitor(pMonitor)
{
    
}

void MonitorThread::run()
{
    int32_t i = 0;
    for (;;)
    {
        if (g_ServiceManager.getServiceCount() == 0) break;

        m_pMonitor->check();
        for (i = 0; i < 5; ++i)
        {
            if (g_ServiceManager.getServiceCount() == 0) break;
            sleep(1);
        }
    }
}
