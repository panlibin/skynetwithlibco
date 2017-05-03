#pragma once

#include "../common/rwlock.h"
#include "../common/circularqueue.h"
#include "message.h"
#include <vector>
#include <map>
#include <string>
#include <assert.h>

namespace csn
{
    class Service;

    class ServiceManager
    {
    public:
        ServiceManager();
        ~ServiceManager();
        
		template<class T>
		uint64_t create()
		{
			m_rwlock.wlock();
			uint64_t ulHandle = 0;
			if (!m_queIdleHandle.pop(ulHandle))
			{
				ulHandle = m_vecService.size();
                assert(ulHandle < MAX_SLOT_SIZE);
                m_vecService.push_back(NULL);
			}

            uint64_t ulIdx = ulHandle & SLOT_MASK;
            assert(ulIdx < m_vecService.size() && m_vecService[ulIdx] == NULL);
            ++m_uServiceCount;
			m_vecService[ulIdx] = new T(ulHandle);
			m_rwlock.wunlock();
			return ulHandle;
		}
        
        template<typename ...Args>
        bool send(uint64_t ulSource, uint64_t ulDestination, eMessageType eType, uint64_t ulSession, Args&&... args)
        {

        }

		Service* grab(uint64_t uHandle);
		void free(uint64_t uHandle);

		void destroy(uint64_t uHandle);
        uint32_t getServiceCount();
	private:
        typedef std::vector<Service*> ServicePtrVec;
        typedef std::map<std::string, uint64_t> NameHandleMap;
        
		RWLock m_rwlock;
		ServicePtrVec m_vecService;
		CircularQueue<uint64_t> m_queIdleHandle;
		NameHandleMap m_mapNameHandle;
        uint32_t m_uServiceCount;

        static const uint64_t MAX_SLOT_SIZE = 0x0000000001000000;
        static const uint64_t SLOT_MASK = MAX_SLOT_SIZE - 1;
        static const uint64_t RECYCLE_INC = MAX_SLOT_SIZE;
        static const uint64_t RECYCLE_MASK = 0x0000FFFFFFFFFFFF;
    };
    
#define g_ServiceManager Singleton<ServiceManager>::instance()
}
