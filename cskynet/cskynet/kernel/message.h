#pragma once

#include "arguments.h"

namespace csn
{
    enum eMessageType
    {
        MTYPE_SEND = 0,
        MTYPE_CALL,
        MTYPE_RESPONSE,
    };
    
    enum eMessageCommand
    {
        MCMD_INIT = 0,
        MCMD_DISPATCH,
        MCMD_EXIT,
    };
    
	struct Message
	{
		uint64_t source;
		uint32_t session;
        int16_t type;
        int16_t cmd;
		Arguments args;
	};
}
