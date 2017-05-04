#pragma once

#include "messagequeue.h"
#include "servicemanager.h"
#include "coroutine.h"
#include <map>
#include <string>

namespace csn
{
    class Coroutine;
    
    class Service
    {
    public:
		Service();
		virtual ~Service();
        
        virtual void init(Arguments& args) = 0;
        virtual void dispatch(Arguments& args) = 0;
        virtual void release(Arguments& args) = 0;

        template<typename ...Args>
        static bool send(uint64_t ulDestination, Args&... args)
        {
            Service* pDst = g_ServiceManager.grab(ulDestination);
            if (NULL == pDst)
            {
                return false;
            }
            Service* pSrc = getThisService();
            Message* pMsg = new Message();
            pMsg->source = pSrc->getHandle();
            pMsg->session = pSrc->allocSessionId();
            pMsg->type = MTYPE_SEND;
            pMsg->cmd = MCMD_DISPATCH;
            wrapper(pMsg->args.push(args)...);
            pDst->pushMessage(pMsg);
            pDst->free();
            return true;
        }

        template<typename ...Args>
        static Arguments& call(uint64_t ulDestination, Args&... args)
        {
            Service* pSrc = getThisService();
            Message* pMsg = new Message();
            pMsg->source = pSrc->getHandle();
            pMsg->session = pSrc->allocSessionId();
            pMsg->type = MTYPE_CALL;
            pMsg->cmd = MCMD_DISPATCH;
            wrapper(pMsg->args.push(args)...);
            return Coroutine::yield(YT_CALL, ulDestination, pMsg);
        }
        
        template<typename ...Args>
        static void ret(Args&... args)
        {
            Service* pSrc = getThisService();
            Service* pDst = g_ServiceManager.grab(pSrc->m_ulMsgSource);
            if (NULL == pDst)
            {
                return;
            }
            Message* pMsg = new Message();
            pMsg->source = pSrc->getHandle();
            pMsg->session = pSrc->m_uMsgSession;
            pMsg->type = MTYPE_RESPONSE;
            pMsg->cmd = MCMD_DISPATCH;
            wrapper(pMsg->args.push(args)...);
            pDst->pushMessage(pMsg);
            pDst->free();
        }

        void callback(Message* pMsg);
        
		void grab();
		void free();
        void setHandle(uint64_t ulHandle);
        uint64_t getHandle();
        const std::string& getName();

        void pushMessage(Message* pMsg);
        static Service* getThisService();
        uint32_t allocSessionId();
    private:
        void suspend(Coroutine* pCo, Arguments& args);
        
        static __thread Service* s_pThisService;
        
        typedef std::map<uint64_t, Coroutine*> CoMapIdxBySession;
        
        MessageQueue* m_pMsgQue;
        uint64_t m_ulHandle;
        uint32_t m_uSession;
        std::string m_strName;
        int32_t m_nRefCnt;
        CoMapIdxBySession m_mapCo;
        uint64_t m_ulMsgSource;
        uint32_t m_uMsgSession;

        int32_t m_nMsgCnt;
        uint64_t m_ulCpuStart;
        uint64_t m_ulCpuCost;
        bool m_bEndless;
        bool m_bProfile;
    };
}
