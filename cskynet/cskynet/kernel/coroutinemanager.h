#pragma once

#include "circularqueue.h"
#include "../common/spinlock.h"
#include "arguments.h"
#include <functional>
#include <map>
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
//        std::map<std::thread::id, ThreadEnv*> m_mappThreadEnv;
        CircularQueue<Coroutine*> m_queCoroutine;
        SpinLock m_lock;
    };
#define g_CoroutineManager Singleton<CoroutineManager>::instance()
}
