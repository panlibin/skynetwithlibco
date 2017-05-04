#include "../common/common.h"
#include "coroutine.h"
#include "coroutinemanager.h"
#include "service.h"
#include "servicemanager.h"

using namespace csn;

ServiceManager::ServiceManager()
    : m_uServiceCount(0)
{
	m_vecService.push_back(NULL);
}

ServiceManager::~ServiceManager()
{

}

Service* ServiceManager::grab(uint64_t ulHandle)
{
	Service* pRes = NULL;
    uint64_t ulIdx = ulHandle & SLOT_MASK;

	m_rwlock.rlock();
    if (ulIdx < m_vecService.size())
    {
        pRes = m_vecService[ulIdx];
    }
	m_rwlock.runlock();

	if (NULL != pRes)
	{
        if (pRes->getHandle() == ulHandle)
        {
            pRes->grab();
        }
        else
        {
            pRes = NULL;
        }
	}

	return pRes;
}

void ServiceManager::free(uint64_t ulHandle)
{
	Service* pRes = NULL;
    uint64_t ulIdx = ulHandle & SLOT_MASK;
    
	m_rwlock.rlock();
    if (ulIdx < m_vecService.size())
    {
        pRes = m_vecService[ulIdx];
    }
	m_rwlock.runlock();

	if (NULL != pRes && pRes->getHandle() == ulHandle)
	{
		pRes->free();
	}
}

void ServiceManager::destroy(uint64_t ulHandle)
{
	Service* pRes = NULL;
    uint64_t ulIdx = ulHandle & SLOT_MASK;
    
	m_rwlock.wlock();
    if (ulIdx < m_vecService.size())
    {
        pRes = m_vecService[ulIdx];
        if (NULL != pRes)
        {
            if (pRes->getHandle() == ulHandle)
            {
                --m_uServiceCount;
                m_vecService[ulIdx] = NULL;
                m_queIdleHandle.push((ulHandle + RECYCLE_INC) & RECYCLE_MASK);
                if (!pRes->getName().empty())
                {
                    NameHandleMap::iterator it = m_mapNameHandle.find(pRes->getName());
                    if (it != m_mapNameHandle.end())
                    {
                        m_mapNameHandle.erase(it);
                    }
                }
            }
            else
            {
                pRes = NULL;
            }
        }
    }
	m_rwlock.wunlock();

    if (NULL != pRes)
    {
        delete pRes;
    }
}

uint32_t ServiceManager::getServiceCount()
{
    return m_uServiceCount;
}

void ServiceManager::initService(Service* pService, uint64_t ulHandle)
{
    pService->setHandle(ulHandle);
    Service* pSrc = Service::getThisService();
    if (NULL != pSrc)
    {
        Message* pMsg = new Message();
        pMsg->source = pSrc->getHandle();
        pMsg->session = pSrc->allocSessionId();
        pMsg->type = MTYPE_CALL;
        pMsg->cmd = MCMD_INIT;
        Coroutine::yield(YT_CALL, ulHandle, pMsg);
    }
    else
    {
        Message* pMsg = new Message();
        pMsg->source = 0;
        pMsg->session = 0;
        pMsg->type = MTYPE_SEND;
        pMsg->cmd = MCMD_INIT;
        pService->pushMessage(pMsg);
        
        pMsg = new Message();
        pMsg->source = 0;
        pMsg->session = 0;
        pMsg->type = MTYPE_SEND;
        pMsg->cmd = MCMD_DISPATCH;
        pService->pushMessage(pMsg);
        
        pMsg = new Message();
        pMsg->source = 0;
        pMsg->session = 0;
        pMsg->type = MTYPE_SEND;
        pMsg->cmd = MCMD_EXIT;
        pService->pushMessage(pMsg);
    }
}
