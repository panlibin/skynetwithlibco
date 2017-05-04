#include "../common/common.h"
#include "service.h"
#include "coroutine.h"
#include "coroutinemanager.h"

using namespace csn;

__thread Service* Service::s_pThisService = NULL;

Service::Service()
	: m_ulHandle(0)
    , m_uSession(0)
	, m_nRefCnt(1)
    , m_ulMsgSource(0)
    , m_uMsgSession(0)
	, m_nMsgCnt(0)
	, m_ulCpuStart(0)
	, m_ulCpuCost(0)
	, m_bEndless(false)
	, m_bProfile(false)
{
	m_pMsgQue = new MessageQueue();
}

Service::~Service()
{
    for (CoMapIdxBySession::iterator it = m_mapCo.begin(); it != m_mapCo.end(); ++it)
    {
        g_CoroutineManager.free(it->second);
    }
    m_pMsgQue->markRelease();
}

void Service::callback(Message* pMsg)
{
    s_pThisService = this;
    switch (pMsg->type)
    {
    case MTYPE_CALL:
    {
        m_ulMsgSource = pMsg->source;
        m_uMsgSession = pMsg->session;
    }
    case MTYPE_SEND:
    {
        Coroutine* pCo = NULL;
        switch (pMsg->cmd)
        {
        case MCMD_DISPATCH:
            pCo = g_CoroutineManager.create(std::bind(&Service::dispatch, this, std::placeholders::_1));
            break;
        case MCMD_INIT:
            pCo = g_CoroutineManager.create(std::bind(&Service::init, this, std::placeholders::_1));
            break;
        case MCMD_EXIT:
            pCo = g_CoroutineManager.create(std::bind(&Service::release, this, std::placeholders::_1));
            break;
        default:
            break;
        }
        suspend(pCo, pCo->resume(pMsg->args));
        break;
    }
    case MTYPE_RESPONSE:
    {
        CoMapIdxBySession::iterator it = m_mapCo.find(pMsg->session);
        if (it != m_mapCo.end())
        {
            Coroutine* pCo = it->second;
            m_mapCo.erase(it);
            suspend(pCo, pCo->resume(pMsg->args));
        }
        break;
    }
    default:
        break;
    }
    delete pMsg;
    m_ulMsgSource = 0;
    m_uMsgSession = 0;
    s_pThisService = NULL;
}

void Service::grab()
{
	ATOM_INC(&m_nRefCnt);
}

void Service::free()
{
	if (ATOM_DEC(&m_nRefCnt) == 0)
	{
		g_ServiceManager.destroy(m_ulHandle);
	}
}

void Service::setHandle(uint64_t ulHandle)
{
    m_ulHandle = ulHandle;
    m_pMsgQue->setHandle(ulHandle);
}

uint64_t Service::getHandle()
{
    return m_ulHandle;
}

const std::string& Service::getName()
{
    return m_strName;
}

void Service::pushMessage(Message* pMsg)
{
    m_pMsgQue->push(pMsg);
}

void Service::suspend(Coroutine* pCo, Arguments& args)
{
    if (args.size() == 0)
    {
        return;
    }
    switch (args.get<int16_t>(0))
    {
    case YT_CALL:
    {
        Service* pDst = g_ServiceManager.grab(args.get<uint64_t>(1));
        Message* pMsg = args.get<Message*>(2);
        if (NULL != pDst)
        {
            assert(m_mapCo.find(pMsg->session) == m_mapCo.end());
            m_mapCo.insert(std::make_pair(pMsg->session, pCo));
            pDst->pushMessage(pMsg);
        }
        else
        {
            g_CoroutineManager.free(pCo);
            delete pMsg;
        }
        pDst->free();
        break;
    }
    default:
        break;
    }
}

uint32_t Service::allocSessionId()
{
    return ATOM_INC(&m_uSession);
}

Service* Service::getThisService()
{
    return s_pThisService;
}
