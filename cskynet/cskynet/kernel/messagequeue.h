#pragma once

#include "message.h"
#include "../common/spinlock.h"
#include "../common/circularqueue.h"

namespace csn
{    
    class MessageQueue
    {
    public:
        MessageQueue();
        ~MessageQueue();

        void push(Message* pMessage);
        bool pop(Message*& pMessage);
        int32_t size();
        
		uint64_t getHandle();
        void markRelease();
        bool isWaitForDestroy();
        void setHandle(uint64_t ulHandle);
    private:
		uint64_t m_ulHandle;
        SpinLock m_lock;
        CircularQueue<Message*> m_queMessage;
        bool m_bInGlobal;
        bool m_bWaitForDestroy;
    };
}
