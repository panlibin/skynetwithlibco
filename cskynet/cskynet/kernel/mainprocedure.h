#pragma once

namespace csn
{
    class MainProcedure
    {
    public:
        void init();
        void start();
        void exit();
    };
    
#define g_MainProcedure Singleton<MainProcedure>::instance()
}
