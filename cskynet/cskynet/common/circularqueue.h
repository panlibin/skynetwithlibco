#pragma once

#include "common.h"
#include <type_traits>
#include <new>
#include <string.h>

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
            m_pQueue = (T*)malloc(sizeof(T) * m_nCap);
        }
        
        CircularQueue(const CircularQueue& que)
        {
            *this = que;
        }
        
        CircularQueue& operator=(const CircularQueue& que)
        {
            if (&que == this)
            {
                return;
            }
            destroyElement(m_nHead, m_nTail);
            free(m_pQueue);
            m_nCap = que.m_nCap;
            m_nSize = que.m_nSize;
            m_nHead = que.m_nHead;
            m_nTail = que.m_nTail;
            m_pQueue = (T*)malloc(sizeof(T) * m_nCap);
            if (m_nHead <= m_nTail)
            {
                for (int32_t i = m_nHead; i < m_nTail; ++i)
                {
                    new (m_pQueue + i)T(que.m_pQueue[i]);
                }
            }
            else
            {
                for (int32_t i = m_nHead; i < m_nCap; ++i)
                {
                    new (m_pQueue + i)T(que.m_pQueue[i]);
                }
                for (int32_t i = 0; i < m_nTail; ++i)
                {
                    new (m_pQueue + i)T(que.m_pQueue[i]);
                }
            }
            return *this;
        }

        ~CircularQueue()
        {
            destroyElement(m_nHead, m_nTail);
            free(m_pQueue);
            m_pQueue = NULL;
        }

        void push(const T& element)
        {
            new (m_pQueue + m_nTail)T(element);
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
                element = m_pQueue[m_nHead];
                destroyElement(m_nHead);
                --m_nSize;
                if (++m_nHead >= m_nCap)
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
            destroyElement(m_nHead, m_nTail);
            m_nSize = 0;
            m_nHead = 0;
            m_nTail = 0;
        }
    protected:
        void expand()
        {
            T* pNewQue = (T*)malloc(sizeof(T) * m_nCap * 2);
            memcpy(pNewQue, m_pQueue + m_nHead, sizeof(T) * (m_nCap - m_nHead));
            if (m_nTail > 0)
            {
                memcpy(pNewQue + m_nCap - m_nHead, m_pQueue, sizeof(T) * m_nTail);
            }
            m_nHead = 0;
            m_nTail = m_nCap;
            m_nCap = m_nCap << 1;
            free(m_pQueue);
            m_pQueue = pNewQue;
        }

        void destroyElement(int32_t nFirst, int32_t nLast)
        {
            if (!std::is_trivially_destructible<T>::value)
            {
                if (nFirst <= nLast)
                {
                    for (int32_t i = nFirst; i < nLast; ++i)
                    {
                        m_pQueue[i].~T();
                    }
                }
                else
                {
                    for (int32_t i = nFirst; i < m_nCap; ++i)
                    {
                        m_pQueue[i].~T();
                    }
                    for (int32_t i = 0; i < nLast; ++i)
                    {
                        m_pQueue[i].~T();
                    }
                }
            }
        }
        
        void destroyElement(int32_t nIdx)
        {
            if (!std::is_trivially_destructible<T>::value)
            {
                m_pQueue[nIdx].~T();
            }
        }

        T* m_pQueue;
        int32_t m_nCap;
        int32_t m_nSize;
        int32_t m_nHead;
        int32_t m_nTail;
        
        static const int32_t DEFAULT_QUEUE_SIZE = 0x40;
    };
}
