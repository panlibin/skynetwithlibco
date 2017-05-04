#pragma once

#include "../common/common.h"
#include "cocontext/coctx.h"
#include "arguments.h"
#include "coroutinemanager.h"
#include <functional>

namespace csn
{
    static const int16_t YT_CALL = 0;
    static const int16_t YT_RESPONSE = 1;
    static const uint32_t DEFAULT_STACK_SIZE = 1024 * 128;
    class Coroutine;

    struct stCoSpec_t
    {
        void *value;
    };
    
    struct stStackMem_t
    {
        Coroutine* occupy_co;
        int stack_size;
        char* stack_bp; //stack_buffer + stack_size
        char* stack_buffer;
        
        stStackMem_t(uint32_t nStackSize)
        {
            occupy_co= NULL;
            stack_size = nStackSize;
            stack_buffer = (char*)malloc(stack_size);
            stack_bp = stack_buffer + stack_size;
        }
        
        ~stStackMem_t()
        {
            free(stack_buffer);
        }
    };
    
    class Coroutine
    {
    public:
        Coroutine();
        ~Coroutine();
        
        void init(const std::function<void(Arguments&)>& fn);
        template<typename... Args>
        Arguments& resume(Args&... args)
        {
            m_args.clear();
            wrapper(m_args.push(args)...);
            return resumeProc();
        }
        
        Arguments& resume(const Arguments& args);
        Arguments& resume();
        
        template<typename... Args>
        static Arguments& yield(Args&... args)
        {
            ThreadEnv* pEnv = g_CoroutineManager.getThreadEnv();
            Arguments& ret = pEnv->pMain->m_args;
            ret.clear();
            wrapper(ret.push(args)...);

            return yieldProc();
        }
        
        static Arguments& yield(const Arguments& args);
        static Arguments& yield();
        
        void run();
        void end();
        void setIsMain(bool bIsMain);
        bool isMain();
    private:
        static Arguments& yieldProc();
        Arguments& resumeProc();
        static void swap(Coroutine* pCurr, Coroutine* pPending);

        coctx_t m_ctx;
        bool m_bStart;
        bool m_bEnd;
        bool m_bIsMain;
        bool m_bEnableSysHook;
        stCoSpec_t m_arrSpec[128];
        stStackMem_t* m_pStackMem;
        std::function<void(Arguments&)> m_fn;
        Arguments m_args;
    };
}
