#include "messagequeue.h"
#include "globalmessagequeue.h"

using namespace csn;

MessageQueue::MessageQueue()
    : m_bInGlobal(false)
    , m_ulHandle(0)
    , m_bWaitForDestroy(false)
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

bool MessageQueue::pop(Message*& pMessage)
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

int32_t MessageQueue::size()
{
    m_lock.lock();
    int32_t nSize = m_queMessage.size();
    m_lock.unlock();
    return nSize;
}

uint64_t MessageQueue::getHandle()
{
	return m_ulHandle;
}

void MessageQueue::markRelease()
{
    m_bWaitForDestroy = true;
}

bool MessageQueue::isWaitForDestroy()
{
    return m_bWaitForDestroy;
}

void MessageQueue::setHandle(uint64_t ulHandle)
{
    m_ulHandle = ulHandle;
}
