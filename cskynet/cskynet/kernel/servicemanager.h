#pragma once

#include "../common/rwlock.h"
#include "../common/circularqueue.h"
#include <vector>
#include <map>
#include <string>

// reserve high 8 bits for remote id
#define HANDLE_MASK 0xffffff
#define HANDLE_REMOTE_SHIFT 24

namespace csn
{
    class Service;

    class ServiceManager
    {
    public:
        ServiceManager();
        ~ServiceManager();
        
		template<class T>
		uint32_t create()
		{
			m_rwlock.wlock();
			uint32_t uHandle = 0;
			if (!m_queIdleHandle.pop(uHandle))
			{
				uHandle = m_vecService.size();
				m_vecService.push_back(NULL);
			}

			m_vecService[uHandle] = new T(uHandle);
			m_rwlock.wunlock();
			return uHandle;
		}

		Service* grab(uint32_t uHandle);
		void free(uint32_t uHandle);

		void destroy(uint32_t uHandle);

		uint64_t allocSessionId();
	private:
		RWLock m_rwlock;
		std::vector<Service*> m_vecService;
		CircularQueue<uint32_t> m_queIdleHandle;
		std::map<std::string, uint32_t> m_mapNameHandle;
		uint64_t m_ulSessionId;
    };
    
#define g_ServiceManager Singleton<ServiceManager>::instance()
}
