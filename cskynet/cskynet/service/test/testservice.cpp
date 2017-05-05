#include "testservice.h"
#include "../../kernel/message.h"

void TestService::init(Arguments& args)
{
    printf("test%d/%d init!\n", args.get<int32_t>(0), args.get<int32_t>(1));
    uint64_t ulHandle = g_Handle.get();
    if (0 == ulHandle)
    {
        Service::call(1, ulHandle);
    }
    else
    {
        Service::call(ulHandle, ulHandle);
    }
    printf("test init finish!\n");
}

class Test
{
    uint64_t a[100];
};

void TestService::dispatch(Arguments& args)
{
    assert(args.get<uint64_t>(0) == ThisService()->getHandle());
//    printf("test%llu dispatch!\n", args.get<uint64_t>(0));
    Service::ret();
    uint64_t ulHandle = g_Handle.get();
    Service::call(ulHandle, ulHandle, Test());
}

void TestService::release(Arguments& args)
{
    printf("test release!\n");
}
