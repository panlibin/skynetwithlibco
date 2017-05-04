#include "coroutine.h"
#include "coroutinemanager.h"

using namespace csn;

extern "C"
{
    extern void coctx_swap( coctx_t *,coctx_t* ) asm("coctx_swap");
};

static int CoRoutineFunc(Coroutine *co, void *)
{
    co->run();
    co->end();
    co->yield();

    return 0;
}

Coroutine::Coroutine()
    : m_bStart(false)
    , m_bEnd(false)
    , m_bIsMain(false)
    , m_bEnableSysHook(true)
{
    m_pStackMem = new stStackMem_t(DEFAULT_STACK_SIZE);
    m_ctx.ss_sp = m_pStackMem->stack_buffer;
    m_ctx.ss_size = DEFAULT_STACK_SIZE;
}

Coroutine::~Coroutine()
{
    delete m_pStackMem;
}

void Coroutine::init(const std::function<void(Arguments&)>& fn)
{
    m_fn = fn;
    m_bStart = false;
    m_bEnd = false;
}

Arguments& Coroutine::resume(const Arguments& args)
{
    m_args = args;
    return resumeProc();
}

Arguments& Coroutine::resume()
{
    m_args.clear();
    return resumeProc();
}

Arguments& Coroutine::resumeProc()
{
    ThreadEnv* pEnv = g_CoroutineManager.getThreadEnv();
    if (!m_bStart)
    {
        coctx_make(&m_ctx, (coctx_pfn_t)CoRoutineFunc, this, NULL);
        m_bStart = true;
    }
    pEnv->pRunning = this;
    swap(pEnv->pMain, this);
    
    pEnv = g_CoroutineManager.getThreadEnv();
    return pEnv->pRunning->m_args;
}

Arguments& Coroutine::yield(const Arguments& args)
{
    ThreadEnv* pEnv = g_CoroutineManager.getThreadEnv();
    pEnv->pMain->m_args = args;

    return yieldProc();
}

Arguments& Coroutine::yield()
{
    ThreadEnv* pEnv = g_CoroutineManager.getThreadEnv();
    pEnv->pMain->m_args.clear();

    return yieldProc();
}

Arguments& Coroutine::yieldProc()
{
    ThreadEnv* pEnv = g_CoroutineManager.getThreadEnv();
    Coroutine* pRunning = pEnv->pRunning;
    pEnv->pRunning = pEnv->pMain;
    swap(pRunning, pEnv->pMain);

    pEnv = g_CoroutineManager.getThreadEnv();
    return pEnv->pRunning->m_args;
}

void Coroutine::swap(Coroutine* pCurr, Coroutine* pPending)
{
    coctx_swap(&(pCurr->m_ctx),&(pPending->m_ctx));
}

void Coroutine::run()
{
    m_fn(m_args);
}

void Coroutine::end()
{
    m_bEnd = true;
}

void Coroutine::setIsMain(bool bIsMain)
{
    m_bIsMain = bIsMain;
}

bool Coroutine::isMain()
{
    return m_bIsMain;
}
