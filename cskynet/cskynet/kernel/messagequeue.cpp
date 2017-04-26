#include "messagequeue.h"
#include "globalmessagequeue.h"

using namespace csn;

MessageQueue::MessageQueue()
    : m_bInGlobal(false)
{
    
}

MessageQueue::~MessageQueue()
{
    
}

void MessageQueue::push(Message* pMessage)
{
    m_lock.lock();
    m_queMessage.push(pMessage);
    if (!m_bInGlobal)
    {
        m_bInGlobal = true;
        g_GlobalMessageQueue.push(this);
    }
    m_lock.unlock();
}

bool MessageQueue::pop(Message* pMessage)
{
    bool suc = false;
    m_lock.lock();
    suc = m_queMessage.pop(pMessage);
    
    if (!suc)
    {
        m_bInGlobal = false;
    }
    m_lock.unlock();
    return suc;
}
