#pragma once

#include "common.h"

namespace csn
{
    template<class T>
    class CircularQueue
    {
    public:
        CircularQueue()
        {
            m_nCap = DEFAULT_QUEUE_SIZE;
            m_nSize = 0;
            m_nHead = 0;
            m_nTail = 0;
            m_pQueue = new T[m_nCap];
        }
        
        CircularQueue(const CircularQueue& que)
        {
            *this = que;
        }
        
        CircularQueue& operator=(const CircularQueue& que)
        {
            m_nCap = que.m_nCap;
            m_nSize = que.m_nSize;
            m_nHead = que.m_nHead;
            m_nTail = que.m_nTail;
            m_pQueue = new T[m_nCap];
            if (m_nHead <= m_nTail)
            {
                for (int32_t i = m_nHead; i < m_nTail; ++i)
                {
                    m_pQueue[i] = que.m_pQueue[i];
                }
            }
            else
            {
                for (int32_t i = 0; i < m_nTail; ++i)
                {
                    m_pQueue[i] = que.m_pQueue[i];
                }
                for (int32_t i = m_nHead; i < m_nCap; ++i)
                {
                    m_pQueue[i] = que.m_pQueue[i];
                }
            }
            return *this;
        }

        ~CircularQueue()
        {
            delete[] m_pQueue;
            m_pQueue = NULL;
        }
        
        void push(const T& element)
        {
            m_pQueue[m_nTail] = element;
            ++m_nSize;
            if (++m_nTail >= m_nCap)
            {
                m_nTail = 0;
            }
            
            if (m_nTail == m_nHead)
            {
                expand();
            }
        }
        
        bool pop(T& element)
        {
            if (!empty())
            {
                element = m_pQueue[m_nHead++];
                --m_nSize;
                if (m_nHead >= m_nCap)
                {
                    m_nHead = 0;
                }
                return true;
            }
            return false;
        }
        
        bool empty()
        {
            return m_nSize == 0;
        }
        
        int32_t size()
        {
            return m_nSize;
        }
        
        void clear()
        {
            m_nCap = DEFAULT_QUEUE_SIZE;
            m_nSize = 0;
            m_nHead = 0;
            m_nTail = 0;
            delete[] m_pQueue;
            m_pQueue = new T[m_nCap];
        }
    protected:
        void expand()
        {
            T* pNewQue = new T[m_nCap << 1];
            for (int32_t i = 0; i < m_nCap; ++i)
            {
                pNewQue[i] = m_pQueue[(m_nHead + i) & m_nCap];
            }
            
            m_nHead = 0;
            m_nTail = m_nCap;
            m_nCap = m_nCap << 1;
            delete [] m_pQueue;
            m_pQueue = pNewQue;
        }
        
        T* m_pQueue;
        int32_t m_nCap;
        int32_t m_nSize;
        int32_t m_nHead;
        int32_t m_nTail;
        
        static const int32_t DEFAULT_QUEUE_SIZE = 0x40;
    };
}
