#include "runnable.h"

using namespace csn;

Runnable::Runnable()
    : m_pThread(NULL)
{
    
}

Runnable::~Runnable()
{
    if (m_pThread)
    {
        delete m_pThread;
    }
}

void Runnable::start()
{
    if (m_pThread)
    {
        return;
    }

    m_pThread = new std::thread(&Runnable::run, this);
}

void Runnable::join()
{
    if (m_pThread)
    {
        m_pThread->join();
    }
}

void Runnable::exit(bool bForce)
{
    if (!bForce)
    {
        join();
    }
    if (m_pThread)
    {
        delete m_pThread;
        m_pThread = NULL;
    }
}
