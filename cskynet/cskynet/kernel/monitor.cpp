#include "../common/common.h"
#include "monitor.h"

using namespace csn;

ThreadMonitor::ThreadMonitor()
	: m_version(0)
	, m_checkVersion(0)
	, m_destination(0)
	, m_source(0)
{

}

ThreadMonitor::~ThreadMonitor()
{

}

void ThreadMonitor::trigger(uint32_t source, uint32_t destination)
{
	m_source = source;
	m_destination = destination;
	ATOM_INC(&m_version);
}

void ThreadMonitor::check()
{
	if (m_version == m_checkVersion)
	{
		if (m_destination)
		{
//			skynet_context_endless(sm->destination);
//			skynet_error(NULL, "A message from [ :%08x ] to [ :%08x ] maybe in an endless loop (version = %d)", sm->source, sm->destination, sm->version);
		}
	}
	else
	{
		m_checkVersion = m_version;
	}
}


Monitor::Monitor()
	: m_nCount(0)
	, m_nSleep(0)
	, m_bQuit(false)
{

}

Monitor::~Monitor()
{

}
