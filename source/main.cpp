#include "GarrysMod/Lua/Interface.h"
#include "GarrysMod/Symbol.hpp"
#include "GarrysMod/InterfacePointers.hpp"
#include "GarrysMod/FunctionPointers.hpp"
#include "scanning/symbolfinder.hpp"

// garrysmod_common has pattern scanning, but it's so fucking unreadable and not documented that I'm just gonna take the YimMenu classes instead.
#include "memory/pattern.hpp"
#include "memory/module.hpp"
#include "memory/byte_patch.hpp"

#include "detouring/helpers.hpp"
#include "detouring/detours.h"

namespace byte_patches {
	static memory::byte_patch* allow_invalid_ticket = nullptr;
	static memory::byte_patch* allow_wrong_game = nullptr;
}

GMOD_MODULE_OPEN()
{
	auto addr = ::memory::module("engine").scan(::memory::pattern("48 8B 95 60 FF FF FF 44")).value();

	byte_patches::allow_invalid_ticket = memory::byte_patch::make(addr.add(46).as<void*>(), {0xE9, 0x16, 0x00, 0x00, 0x00}).get();
	byte_patches::allow_wrong_game = memory::byte_patch::make(addr.sub(27).as<void*>(), {0xE9, 0x5F, 0x00, 0x00, 0x00}).get();

	byte_patches::allow_invalid_ticket->apply();
	byte_patches::allow_wrong_game->apply();
	Msg("Allow invalid ticket patch applied.\n");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	if (byte_patches::allow_invalid_ticket)
		byte_patches::allow_invalid_ticket->restore();
	if (byte_patches::allow_wrong_game)
		byte_patches::allow_wrong_game->restore();

	return 0;
}
