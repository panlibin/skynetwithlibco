#include "../common/common.h"
#include "mainprocedure.h"
#include "monitorthread.h"
#include "timerthread.h"
#include "workthread.h"
#include "monitor.h"

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
}

void MainProcedure::exit()
{
    
}
