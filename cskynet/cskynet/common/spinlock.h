#pragma once

#include "common.h"

namespace csn
{
    class SpinLock
    {
    public:
        SpinLock()
        {
            m_nLock = 0;
        }
        
        ~SpinLock()
        {
            m_nLock = 0;
        }
        
        void lock()
        {
            while (__sync_lock_test_and_set(&m_nLock, 1)) {}
        }
        
        void unlock()
        {
            __sync_lock_release(&m_nLock);
        }
        
        bool trylock()
        {
            return __sync_lock_test_and_set(&m_nLock, 1) == 0;
        }

    private:
        int32_t m_nLock;

    };
}
