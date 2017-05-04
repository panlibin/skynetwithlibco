#pragma once

#include "runnable.h"

namespace csn
{
    class Monitor;
    class ThreadMonitor;
    class MessageQueue;
    
    class WorkThread : public Runnable
    {
    public:
        WorkThread(Monitor* pMonitor, ThreadMonitor* pThreadMonitor, int32_t nWeight);
        virtual void run();
        
    private:
        MessageQueue* dispatchMessage(MessageQueue* pQue);
        
        Monitor* m_pMonitor;
        ThreadMonitor* m_pThreadMonitor;
        int32_t m_nWeight;
    };
}
