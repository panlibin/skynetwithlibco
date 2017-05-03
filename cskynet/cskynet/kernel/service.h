#pragma once

#include "messagequeue.h"
#include <map>
#include <string>

namespace csn
{
    class Coroutine;
    
    class Service
    {
    public:
		Service(uint64_t ulHandle);
		virtual ~Service();

		virtual void init(Arguments& args) = 0;
        virtual void dispatch(Arguments& args) = 0;
		virtual void release(Arguments& args) = 0;

        void callback(Message* pMsg);
        
		void grab();
		void free();
        uint64_t getHandle();
        const std::string& getName();
        
        void pushMessage(Message* pMsg);
    private:
        MessageQueue* m_pMsgQue;
        uint64_t m_ulHandle;
        uint64_t m_ulSession;
        std::string m_strName;
        int32_t m_nRefCnt;
        std::map<uint64_t, Coroutine*> m_mapCo;

        int32_t m_nMsgCnt;
        uint64_t m_ulCpuStart;
        uint64_t m_ulCpuCost;
        bool m_bEndless;
        bool m_bProfile;
    };
}
