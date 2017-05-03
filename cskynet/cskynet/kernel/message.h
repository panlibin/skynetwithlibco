#pragma once

#include "arguments.h"

namespace csn
{
    enum eMessageType
    {
        PTYPE_CALL = 0,
        PTYPE_SEND,
        PTYPE_RESPONSE,
    };
    
    enum eMessageCommand
    {
        CTYPE_INIT = 0,
        CTYPE_DISPATCH,
        CTYPE_EXIT,
    };
    
	struct Message
	{
		uint64_t source;
		uint64_t session;
        int16_t type;
        int16_t cmd;
		Arguments args;
	};
}
