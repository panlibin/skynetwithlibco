#include "../common/common.h"
#include "service.h"
#include "servicemanager.h"

using namespace csn;

ServiceManager::ServiceManager()
	: m_ulSessionId(0)
{
	m_vecService.push_back(NULL);
}

ServiceManager::~ServiceManager()
{

}

Service* ServiceManager::grab(uint32_t uHandle)
{
	Service* pRes = NULL;
	m_rwlock.rlock();
	pRes = m_vecService[uHandle];
	m_rwlock.runlock();

	if (pRes)
	{
		pRes->grab();
	}

	return pRes;
}

void ServiceManager::free(uint32_t uHandle)
{
	Service* pRes = NULL;
	m_rwlock.rlock();
	pRes = m_vecService[uHandle];
	m_rwlock.runlock();

	if (pRes)
	{
		pRes->free();
	}
}

void ServiceManager::destroy(uint32_t uHandle)
{
	Service* pRes = NULL;
	m_rwlock.wlock();
	pRes = m_vecService[uHandle];
	m_vecService[uHandle] = NULL;
	m_queIdleHandle.push(uHandle);
	m_rwlock.wunlock();

	delete pRes;
}

uint64_t ServiceManager::allocSessionId()
{
	return ATOM_INC(&m_ulSessionId);
}
