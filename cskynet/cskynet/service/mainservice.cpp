#include "mainservice.h"
#include "logger/logservice.h"
#include "test/testservice.h"

using namespace csn;

void Main::init(Arguments& args)
{
    printf("Main init!\n");
    uint64_t ulLogHandle = Service::newservice<LogService>();
    Service::call(ulLogHandle);
    
    for (int32_t i = 0; i < 100; ++i)
    {
        g_Handle.set(Service::newservice<TestService>(i, 100));
    }
    
    Service::exit();
}

void Main::dispatch(Arguments& args)
{
    printf("Main dispatch!\n");
    Service::ret();
}

void Main::release(Arguments& args)
{
    printf("Main exit!\n");
}
