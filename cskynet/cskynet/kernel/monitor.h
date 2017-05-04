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

		void trigger(uint64_t source, uint64_t destination);
		void check();
	private:
		int32_t m_version;
		int32_t m_checkVersion;
		uint64_t m_source;
		uint64_t m_destination;
	};

    class Monitor
    {
	public:
		Monitor(int32_t nThreadCount);
		~Monitor();

        void wakeup(int32_t busy);
        int32_t getThreadCount();
        bool quit();
        ThreadMonitor* getThreadMonitor(int32_t nIdx);
        void check();
        void wait();
	private:
		int32_t m_nCount;
		int32_t m_nSleep;
		bool m_bQuit;
		std::vector<ThreadMonitor*> m_vecThreadMonitor;
		std::condition_variable m_cond;
		std::mutex m_mutex;
    };
}
