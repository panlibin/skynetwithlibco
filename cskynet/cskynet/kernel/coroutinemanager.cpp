#include "coroutine.h"
#include "coroutinemanager.h"

using namespace csn;

__thread ThreadEnv* CoroutineManager::m_pThreadEnv = NULL;

ThreadEnv* CoroutineManager::getThreadEnv()
{
    if (NULL == m_pThreadEnv)
    {
        m_pThreadEnv = new ThreadEnv();
        m_pThreadEnv->pMain = new Coroutine();
        m_pThreadEnv->pMain->disableHookSys();
        m_pThreadEnv->pMain->setIsMain(true);
        m_pThreadEnv->pRunning = m_pThreadEnv->pMain;
    }
    return m_pThreadEnv;
}

Coroutine* CoroutineManager::create(const std::function<void(Arguments&)>& fn)
{
    Coroutine* pCo = NULL;
    m_lock.lock();
    m_queCoroutine.pop(pCo);
    m_lock.unlock();
    
    if (NULL == pCo)
    {
        pCo = new Coroutine();
    }
    pCo->init(fn);

    return pCo;
}

void CoroutineManager::free(Coroutine* pCo)
{
    m_lock.lock();
    m_queCoroutine.push(pCo);
    m_lock.unlock();
}
