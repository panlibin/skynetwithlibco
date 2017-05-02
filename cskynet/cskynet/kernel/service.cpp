#include "../common/common.h"
#include "servicemanager.h"
#include "service.h"

using namespace csn;

Service::Service(uint32_t uHandle)
	: m_uHandle(uHandle)
	, m_nRefCnt(1)
	, m_nMsgCnt(0)
	, m_ulCpuStart(0)
	, m_ulCpuCost(0)
	, m_bEndless(false)
	, m_bProfile(false)
{
	m_pMsgQue = new MessageQueue(m_uHandle);
}

Service::~Service()
{

}

void Service::grab()
{
	ATOM_INC(&m_nRefCnt);
}

void Service::free()
{
	if (ATOM_DEC(&m_nRefCnt) == 0)
	{
		g_ServiceManager.destroy(m_uHandle);
	}
}
