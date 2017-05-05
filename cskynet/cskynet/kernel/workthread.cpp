#include "../common/common.h"
#include "monitor.h"
#include "workthread.h"
#include "messagequeue.h"
#include "globalmessagequeue.h"
#include "service.h"
#include "servicemanager.h"

using namespace csn;

WorkThread::WorkThread(Monitor* pMonitor, ThreadMonitor* pThreadMonitor, int32_t nWeight)
    : m_pMonitor(pMonitor)
    , m_pThreadMonitor(pThreadMonitor)
    , m_nWeight(nWeight)
{

}

void WorkThread::run()
{
    MessageQueue* pQue = NULL;
    while (!m_pMonitor->isQuit())
    {
        pQue = dispatchMessage(pQue);
        if (pQue == NULL)
        {
            m_pMonitor->wait();
        }
    }
}

MessageQueue* WorkThread::dispatchMessage(MessageQueue* pQue)
{
    if (pQue == NULL)
    {
        pQue = g_GlobalMessageQueue.pop();
        if (pQue == NULL)
        {
            return NULL;
        }
    }
    
    if (pQue->isWaitForDestroy())
    {
        delete pQue;
        return g_GlobalMessageQueue.pop();
    }
    Service* pService = g_ServiceManager.grab(pQue->getHandle());
    if (pService == NULL)
    {
        delete pQue;
        return g_GlobalMessageQueue.pop();
    }
    
    int32_t n = 1;
    Message* pMsg = NULL;
    for (int32_t i = 0; i < n; ++i)
    {
        if (!pQue->pop(pMsg))
        {
            pService->free();
            return g_GlobalMessageQueue.pop();
        }
        else if (i == 0 && m_nWeight >= 0)
        {
            n = pQue->size();
            n >>= m_nWeight;
        }
        
        m_pThreadMonitor->trigger(pMsg->source, pQue->getHandle());
        
        pService->callback(pMsg);
        
        m_pThreadMonitor->trigger(0, 0);
    }
    
    MessageQueue* pNextQue = g_GlobalMessageQueue.pop();
    if (pNextQue)
    {
        g_GlobalMessageQueue.push(pQue);
        pQue = pNextQue;
    }
    pService->free();
    return pQue;
}
