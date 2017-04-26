#pragma once

namespace csn
{
    class ServiceManager
    {
    public:
        ServiceManager();
        ~ServiceManager();
        
        
    };
    
#define g_ServiceManager Singleton<ServiceManager>::instance()
}
