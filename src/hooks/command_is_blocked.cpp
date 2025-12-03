#include "hooking/hooking.hpp"
#include <tier0/dbg.h>

namespace gm
{
	char* hooks::ConCommand_IsBlocked(char* command)
	{
		return nullptr;
	}
}
