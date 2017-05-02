#pragma once

#include <vector>
#include <condition_variable>
#include <mutex>

namespace csn
{
	class ThreadMonitor
	{
	public:
		ThreadMonitor();
		~ThreadMonitor();

		void trigger(uint32_t source, uint32_t destination);
		void check();
	private:
		int32_t m_version;
		int32_t m_checkVersion;
		uint32_t m_source;
		uint32_t m_destination;
	};

    class Monitor
    {
	public:
		Monitor();
		~Monitor();

	private:
		int32_t m_nCount;
		int32_t m_nSleep;
		bool m_bQuit;
		std::vector<ThreadMonitor*> m_vecThreadMonitor;
		std::condition_variable m_cond;
		std::mutex m_mutex;
    };
}
