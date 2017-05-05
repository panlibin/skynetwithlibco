#pragma once

#include "../../kernel/service.h"
#include "../../common/rwlock.h"
#include <vector>

using namespace csn;

static uint32_t idx = 0;

struct ServiceHandle
{
    uint64_t get()
    {
        m_lock.rlock();
        uint64_t ulHandle = 0;
        if (!m_vecHandle.empty())
        {
            ulHandle = m_vecHandle[ATOM_INC(&idx) % m_vecHandle.size()];
        }
        m_lock.runlock();
        return ulHandle;
    }
    
    void set(uint64_t ulHandle)
    {
        m_lock.wlock();
        m_vecHandle.push_back(ulHandle);
        m_lock.wunlock();
    }
    
    std::vector<uint64_t> m_vecHandle;
    RWLock m_lock;
};
#define g_Handle csn::Singleton<ServiceHandle>::instance()

class TestService : public Service
{
public:
    virtual void init(Arguments& args);
    virtual void dispatch(Arguments& args);
    virtual void release(Arguments& args);
};
