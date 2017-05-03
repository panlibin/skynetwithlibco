#pragma once

#include "runnable.h"

namespace csn
{
    class Monitor;
    
    class MonitorThread : public Runnable
    {
    public:
        MonitorThread(Monitor* pMonitor);
        virtual void run();

    private:
        Monitor* m_pMonitor;
    };
}
