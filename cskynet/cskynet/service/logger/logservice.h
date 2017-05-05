#pragma once

#include "../../kernel/service.h"

namespace csn
{
    class LogService : public Service
    {
    public:
        virtual void init(Arguments& args);
        virtual void dispatch(Arguments& args);
        virtual void release(Arguments& args);
    };
}
