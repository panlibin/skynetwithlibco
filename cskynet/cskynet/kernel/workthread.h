#pragma once

#include "runnable.h"

namespace csn
{
    class WorkThread : public Runnable
    {
    public:
        virtual void run();
        
    private:
        int32_t m_nId;
    };
}
