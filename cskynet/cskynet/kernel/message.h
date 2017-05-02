#pragma once

#include "arguments.h"

namespace csn
{
	struct Message
	{
		uint32_t source;
		int32_t session;
		Arguments args;
	};
}
