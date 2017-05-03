#pragma once

#include "runnable.h"

namespace csn
{
    class Monitor;
    
    class TimerThread : public Runnable
    {
    public:
        TimerThread(Monitor* pMonitor);
        virtual void run();
    private:
        Monitor* m_pMonitor;
    };
}
