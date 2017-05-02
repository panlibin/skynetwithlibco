#pragma once

#include "messagequeue.h"
#include <map>

namespace csn
{
    class Service
    {
    public:
		Service(uint32_t uHandle);
		virtual ~Service();

		virtual void init(const Message& msg) = 0;
        virtual void dispatch(const Message& msg) = 0;
		virtual void release(const Message& msg) = 0;

		void grab();
		void free();

    private:
        MessageQueue* m_pMsgQue;
        uint32_t m_uHandle;
        int32_t m_nRefCnt;

        int32_t m_nMsgCnt;
        uint64_t m_ulCpuStart;
        uint64_t m_ulCpuCost;
        bool m_bEndless;
        bool m_bProfile;
    };
}
