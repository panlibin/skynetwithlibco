#pragma once

#include <functional>
#include "messagequeue.h"

namespace csn
{
    class Service
    {
    public:
        Service();
        ~Service();
        
        virtual void callback(Message& msg) = 0;
        
    private:
        MessageQueue* m_pMsgQue;
        uint32_t m_uHandle;
        uint32_t m_uSessionId;
        int32_t m_nRefCnt;

        int32_t m_nMsgCnt;
        uint64_t m_ulCpuStart;
        uint64_t m_ulCpuCost;
        bool m_bEndless;
        bool m_bProfile;
    };
}
