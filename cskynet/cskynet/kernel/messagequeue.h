#pragma once

#include "message.h"
#include "../common/spinlock.h"
#include "../common/circularqueue.h"

namespace csn
{    
    class MessageQueue
    {
    public:
        MessageQueue(uint64_t ulHandle);
        ~MessageQueue();

        void push(Message* pMessage);
        bool pop(Message* pMessage);

		uint64_t getHandle();
    private:
		uint64_t m_ulHandle;
        SpinLock m_lock;
        CircularQueue<Message*> m_queMessage;
        bool m_bInGlobal;
    };
}
