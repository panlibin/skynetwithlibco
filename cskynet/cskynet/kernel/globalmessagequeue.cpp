#include "../common/common.h"
#include "globalmessagequeue.h"

using namespace csn;

void GlobalMessageQueue::push(MessageQueue* que)
{
    m_lock.lock();
    m_queMQ.push(que);
    m_lock.unlock();
}

MessageQueue* GlobalMessageQueue::pop()
{
    MessageQueue* pMQ = NULL;
    m_lock.lock();
//    if (!m_queMQ.empty())
//    {
//        pMQ = m_queMQ.front();
//        m_queMQ.pop();
//    }
    m_queMQ.pop(pMQ);
    m_lock.unlock();
    return pMQ;
}
