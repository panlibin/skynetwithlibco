#pragma once

#include "arguments.h"
#include "../common/spinlock.h"
#include "../common/circularqueue.h"

#define MESSAGE_TYPE_MASK (SIZE_MAX >> 8)
#define MESSAGE_TYPE_SHIFT ((sizeof(size_t)-1) * 8)

namespace csn
{
    struct Message
    {
        uint32_t source;
        int32_t session;
        Arguments args;
    };
    
    class MessageQueue
    {
    public:
        MessageQueue();
        ~MessageQueue();

        void push(Message* pMessage);
        bool pop(Message* pMessage);

    private:
        SpinLock m_lock;
        CircularQueue<Message*> m_queMessage;
        bool m_bInGlobal;
    };
}
