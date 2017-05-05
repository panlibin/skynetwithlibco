#pragma once

#include "../common/spinlock.h"
#include "messagequeue.h"
#include "../common/circularqueue.h"

namespace csn
{
    class GlobalMessageQueue
    {
    public:
        void push(MessageQueue* que);
        MessageQueue* pop();
    private:
        SpinLock m_lock;
        CircularQueue<MessageQueue*> m_queMQ;
    };
}
#define g_GlobalMessageQueue csn::Singleton<csn::GlobalMessageQueue>::instance()
