#pragma once

#include "../common/spinlock.h"
#include "messagequeue.h"
#include "../common/circularqueue.h"
//#include <queue>

namespace csn
{
    class GlobalMessageQueue
    {
    public:
        void push(MessageQueue* que);
        MessageQueue* pop();
    private:
        SpinLock m_lock;
//        std::queue<MessageQueue*> m_queMQ;
        CircularQueue<MessageQueue*> m_queMQ;
    };

#define g_GlobalMessageQueue Singleton<GlobalMessageQueue>::instance()
}
