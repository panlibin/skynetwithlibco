#pragma once

#include <thread>

namespace csn
{
    class Runnable
    {
    public:
        Runnable();
        virtual ~Runnable();

        virtual void run() = 0;
        
        void start();
        void join();
        void exit(bool bForce = false);
    private:
        std::thread* m_pThread;
    };
}
