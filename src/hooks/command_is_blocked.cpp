#include "hooking/hooking.hpp"

namespace gm
{
	char* hooks::ConCommand_IsBlocked(char* command)
	{
		return nullptr;
	}
}
