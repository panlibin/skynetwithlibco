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

        uint64_t registerHandle(Service* pService);
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
}
#define g_ServiceManager csn::Singleton<csn::ServiceManager>::instance()
