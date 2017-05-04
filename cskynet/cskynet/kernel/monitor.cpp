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

void ThreadMonitor::trigger(uint64_t source, uint64_t destination)
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


Monitor::Monitor(int32_t nThreadCount)
	: m_nCount(nThreadCount)
	, m_nSleep(0)
	, m_bQuit(false)
{
    m_vecThreadMonitor.resize(m_nCount, NULL);
    for (int32_t i = 0; i < m_nCount; ++i)
    {
        m_vecThreadMonitor[i] = new ThreadMonitor();
    }
}

Monitor::~Monitor()
{
    for (int32_t i = 0; i < m_vecThreadMonitor.size(); ++i)
    {
        delete m_vecThreadMonitor[i];
    }
}

void Monitor::wakeup(int32_t nBusy)
{
    if (m_nSleep >= m_nCount - nBusy)
    {
        m_cond.notify_one();
    }
}

int32_t Monitor::getThreadCount()
{
    return m_nCount;
}

bool Monitor::quit()
{
    return m_bQuit;
}

ThreadMonitor* Monitor::getThreadMonitor(int32_t nIdx)
{
    return m_vecThreadMonitor[nIdx];
}

void Monitor::check()
{
    for (int32_t i = 0; i < m_vecThreadMonitor.size(); ++i)
    {
        m_vecThreadMonitor[i]->check();
    }
}

void Monitor::wait()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    ++m_nSleep;
    if (!quit())
    {
        m_cond.wait(lock);
    }
    --m_nSleep;
}
