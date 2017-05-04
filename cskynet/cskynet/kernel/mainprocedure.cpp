#include "../common/common.h"
#include "mainprocedure.h"
#include "monitorthread.h"
#include "timerthread.h"
#include "workthread.h"
#include "monitor.h"
#include "../service/mainservice.h"
#include "servicemanager.h"

using namespace csn;

void MainProcedure::init()
{
    printf("init\n");
}

void MainProcedure::start()
{
    std::vector<Runnable*> vecThread;
    vecThread.resize(8 + 3, NULL);
    
    Monitor* pMonitor = new Monitor(8);
    
    vecThread[0] = new MonitorThread(pMonitor);
    vecThread[1] = new TimerThread(pMonitor);
    vecThread[2] = new WorkThread(pMonitor, pMonitor->getThreadMonitor(0), -1);
    
    g_ServiceManager.create<Main>();

    for (int32_t i = 0; i < 3; ++i)
    {
        vecThread[i]->start();
    }
    for (int32_t i = 0; i < 3; ++i)
    {
        vecThread[i]->join();
    }
}

void MainProcedure::exit()
{
    
}
