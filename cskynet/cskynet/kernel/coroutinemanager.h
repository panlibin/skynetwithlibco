#pragma once

#include "../common/circularqueue.h"
#include "../common/spinlock.h"
#include "arguments.h"
#include <functional>
#include <thread>

namespace csn
{
    class Coroutine;
    
    struct ThreadEnv
    {
        Coroutine* pMain;
        Coroutine* pRunning;
    };
    
    class CoroutineManager
    {
    public:
        ThreadEnv* getThreadEnv();
        Coroutine* create(const std::function<void(Arguments&)>& fn);
        void free(Coroutine* pCo);
    private:
        static __thread ThreadEnv* m_pThreadEnv;
        CircularQueue<Coroutine*> m_queCoroutine;
        SpinLock m_lock;
    };
}
#define g_CoroutineManager csn::Singleton<csn::CoroutineManager>::instance()
