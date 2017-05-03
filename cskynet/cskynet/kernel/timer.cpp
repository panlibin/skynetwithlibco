#include "../common/common.h"
#include <time.h>
#include <assert.h>
#if defined(__APPLE__)
#include <sys/time.h>
#include <mach/task.h>
#include <mach/mach.h>
#endif
#include "timer.h"

using namespace csn;

Timer::Timer()
    : m_ulCurTimerId(0)
    , m_uTime(0)
{
    systime(m_ulStartTime, m_ulCurrent);
    m_ulCurrentPoint = gettime();
}

void Timer::updateTime()
{
    uint64_t cp = gettime();
    if (cp < m_ulCurrentPoint)
    {
        //add code
        m_ulCurrentPoint = cp;
    }
    else
    {
        uint64_t diff = cp - m_ulCurrentPoint;
        m_ulCurrentPoint = cp;
        m_ulCurrent += diff;
        for (int32_t i = 0; i < diff; ++i)
        {
            update();
        }
    }
}

uint64_t Timer::timeout(uint64_t ulHandle, uint64_t ulSession, uint32_t uTime, bool bLoop)
{
    uint64_t ulTimerId = ATOM_INC(&m_ulCurTimerId);
    if (uTime <= 0)
    {
        //add code
    }
    else
    {
        TimerEvent* pEvent = new TimerEvent();
        pEvent->ulTimerId = ulTimerId;
        pEvent->ulHandle = ulHandle;
        pEvent->ulSession = ulSession;
        pEvent->uInterval = uTime;
        pEvent->bLoop = bLoop;
        m_lock.lock();
        pEvent->uExpire = uTime + m_uTime;
        addNode(pEvent);
        m_lock.unlock();
    }
    return ulTimerId;
}

uint64_t Timer::gettime()
{
    uint64_t t;
#if !defined(__APPLE__)
    struct timespec ti;
    clock_gettime(CLOCK_MONOTONIC, &ti);
    t = (uint64_t)ti.tv_sec * 1000;
    t += ti.tv_nsec / 1000000;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    t = (uint64_t)tv.tv_sec * 1000;
    t += tv.tv_usec / 1000;
#endif
    return t;
}

void Timer::systime(uint64_t& sec, uint64_t& msec)
{
#if !defined(__APPLE__)
    struct timespec ti;
    clock_gettime(CLOCK_REALTIME, &ti);
    sec = (uint64_t)ti.tv_sec;
    msec = (uint64_t)(ti.tv_nsec / 1000000);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    sec = tv.tv_sec;
    msec = tv.tv_usec / 1000;
#endif
}

void Timer::update()
{
    m_lock.lock();
    execute();
    shift();
    execute();
    m_lock.unlock();
}

void Timer::execute()
{
    int32_t nIdx = m_uTime & TIME_NEAR_MASK;
    
    while (!m_lstNear[nIdx].empty())
    {
        m_lstNear[nIdx].swap(m_lstTemp);
        m_lock.unlock();
        dispatch();
        m_lock.lock();
    }
}

void Timer::shift()
{
    uint32_t uMask = TIME_NEAR;
    ++m_uTime;
    if (m_uTime == 0)
    {
        hashEvent(3, 0);
    }
    else
    {
        uint32_t time = m_uTime >> TIME_NEAR_SHIFT;
        int32_t i = 0;
        while ((m_uTime & (uMask - 1)) == 0)
        {
            int32_t idx = time & TIME_LEVEL_MASK;
            if (idx != 0)
            {
                hashEvent(i, idx);
                break;
            }
            uMask <<= TIME_LEVEL_SHIFT;
            time >>= TIME_LEVEL_SHIFT;
            ++i;
        }
    }
}

void Timer::dispatch()
{
    //add code
}

void Timer::addNode(TimerEvent* pEvent)
{
    uint32_t time = pEvent->uExpire;
    if ((time | TIME_NEAR_MASK) == (m_uTime | TIME_NEAR_MASK))
    {
        m_lstNear[time & TIME_NEAR_MASK].push_back(pEvent);
    }
    else
    {
        uint32_t uMask = TIME_NEAR << TIME_LEVEL_SHIFT;
        int32_t i;
        for (i = 0; i < 3; ++i)
        {
            if ((time | (uMask - 1)) == (m_uTime | (uMask - 1)))
            {
                break;
            }
            uMask <<= TIME_LEVEL_SHIFT;
        }
        m_lstLevel[i][((time >> (TIME_NEAR_SHIFT + i * TIME_LEVEL_SHIFT)) & TIME_LEVEL_MASK)].push_back(pEvent);
    }
}

void Timer::hashEvent(int32_t level, int32_t idx)
{
    TimerEventPtrList& lstEvent = m_lstLevel[level][idx];
    for (TimerEventPtrList::iterator it = lstEvent.begin(); it != lstEvent.end(); ++it)
    {
        addNode(*it);
    }
    lstEvent.clear();
}
