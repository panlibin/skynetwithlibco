#pragma once

#include "message.h"
#include "../common/spinlock.h"
#include "../common/circularqueue.h"

#define MESSAGE_TYPE_MASK (SIZE_MAX >> 8)
#define MESSAGE_TYPE_SHIFT ((sizeof(size_t)-1) * 8)

namespace csn
{    
    class MessageQueue
    {
    public:
        MessageQueue(uint32_t uHandle);
        ~MessageQueue();

        void push(Message* pMessage);
        bool pop(Message* pMessage);

		uint32_t getHandle();
    private:
		uint32_t m_uHandle;
        SpinLock m_lock;
        CircularQueue<Message*> m_queMessage;
        bool m_bInGlobal;
    };
}
