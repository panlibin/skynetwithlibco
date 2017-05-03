#include "../common/common.h"
#include "servicemanager.h"
#include "service.h"
#include "coroutine.h"
#include "coroutinemanager.h"

using namespace csn;

Service::Service(uint64_t ulHandle)
	: m_ulHandle(ulHandle)
    , m_ulSession(0)
	, m_nRefCnt(1)
	, m_nMsgCnt(0)
	, m_ulCpuStart(0)
	, m_ulCpuCost(0)
	, m_bEndless(false)
	, m_bProfile(false)
{
	m_pMsgQue = new MessageQueue(m_ulHandle);
}

Service::~Service()
{

}

void Service::callback(Message* pMsg)
{
    switch (pMsg->type)
    {
    case PTYPE_CALL:
    case PTYPE_SEND:
    {
        Coroutine* pCo = NULL;
        switch (pMsg->cmd)
        {
        case CTYPE_DISPATCH:
            pCo = g_CoroutineManager.create(std::bind(&Service::dispatch, this, std::placeholders::_1));
            break;
        case CTYPE_INIT:
            pCo = g_CoroutineManager.create(std::bind(&Service::init, this, std::placeholders::_1));
            break;
        case CTYPE_EXIT:
            pCo = g_CoroutineManager.create(std::bind(&Service::release, this, std::placeholders::_1));
            break;
        default:
            break;
        }
        pCo->resume(pMsg);
        break;
    }
    case PTYPE_RESPONSE:
            //add code
        break;
    default:
        break;
    }
    delete pMsg;
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
