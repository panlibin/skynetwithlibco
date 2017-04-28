#pragma once

// reserve high 8 bits for remote id
#define HANDLE_MASK 0xffffff
#define HANDLE_REMOTE_SHIFT 24

namespace csn
{
    class Service;

    class ServiceManager
    {
    public:
        ServiceManager();
        ~ServiceManager();
        
        
    };
    
#define g_ServiceManager Singleton<ServiceManager>::instance()
}
