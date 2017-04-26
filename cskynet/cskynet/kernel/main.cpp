#include "../common/common.h"
#include "mainprocedure.h"

using namespace csn;

int32_t main(int32_t argc, char* argv[])
{
    g_MainProcedure.init();
    g_MainProcedure.start();
    g_MainProcedure.exit();

    return 0;
}
