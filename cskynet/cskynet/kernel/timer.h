#pragma once

#include <map>
#include <list>
#include "../common/spinlock.h"

namespace csn
{
    struct TimerEvent
    {
        uint64_t ulTimerId;
        uint64_t ulHandle;
        uint64_t ulSession;
        uint32_t uInterval;
        uint32_t uExpire;
        bool bLoop;
    };
    
    class Timer
    {
    public:
        Timer();
        
        void updateTime();
        uint64_t timeout(uint64_t ulHandle, uint64_t ulSession, uint32_t uTime, bool bLoop);
        
    private:
        static uint64_t gettime();
        static void systime(uint64_t& sec, uint64_t& msec);
        
        void update();
        void execute();
        void shift();
        void dispatch();
        void addNode(TimerEvent* pEvent);
        void hashEvent(int32_t level, int32_t idx);

        static const uint64_t TIME_NEAR_SHIFT = 8;
        static const uint64_t TIME_NEAR = 1 << TIME_NEAR_SHIFT;
        static const uint64_t TIME_LEVEL_SHIFT = 6;
        static const uint64_t TIME_LEVEL = 1 << TIME_LEVEL_SHIFT;
        static const uint64_t TIME_NEAR_MASK = TIME_NEAR - 1;
        static const uint64_t TIME_LEVEL_MASK = TIME_LEVEL - 1;
        
        typedef std::list<TimerEvent*> TimerEventPtrList;
        
        uint64_t m_ulCurTimerId;
        uint64_t m_ulStartTime;
        uint64_t m_ulCurrent;
        uint64_t m_ulCurrentPoint;
        uint32_t m_uTime;
        SpinLock m_lock;
        TimerEventPtrList m_lstNear[TIME_NEAR];
        TimerEventPtrList m_lstLevel[4][TIME_LEVEL];
        TimerEventPtrList m_lstTemp;
    };
#define g_Timer Singleton<Timer>::instance()
}
