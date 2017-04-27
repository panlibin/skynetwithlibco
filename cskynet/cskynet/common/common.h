#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "singleton.h"

namespace csn
{
    template<typename ...Args>
    void wrapper(Args... args) {}
}
