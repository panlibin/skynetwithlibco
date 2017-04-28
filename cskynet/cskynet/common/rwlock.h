#pragma once

namespace csn
{
    class RWLock
    {
    public:
        RWLock()
            : m_nWrite(0)
            , m_nRead(0)
        {
            
        }
        
        ~RWLock()
        {
            m_nWrite = 0;
            m_nRead = 0;
        }
        
        void rlock()
        {
            for (;;)
            {
                while (m_nWrite)
                {
                    __sync_synchronize();
                }
                __sync_add_and_fetch(&m_nRead, 1);
                if (m_nWrite)
                {
                    __sync_sub_and_fetch(&m_nRead, 1);
                }
                else
                {
                    break;
                }
            }
        }
        
        void runlock()
        {
            __sync_sub_and_fetch(&m_nRead, 1);
        }
        
        void wlock()
        {
            while (__sync_lock_test_and_set(&m_nWrite, 1)) {}
            while (m_nRead)
            {
                __sync_synchronize();
            }
        }
        
    private:
        int32_t m_nWrite;
        int32_t m_nRead;
    };
}
