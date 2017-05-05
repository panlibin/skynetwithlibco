#include "logservice.h"

using namespace csn;

void LogService::init(Arguments& args)
{
    printf("log init!\n");
}

void LogService::dispatch(Arguments& args)
{
    printf("log dispatch!\n");
    Service::ret();
    Service::exit();
}

void LogService::release(Arguments& args)
{
    printf("log release!\n");
}
