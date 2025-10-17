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

static memory::byte_patch* allow_invalid_ticket = nullptr;
static memory::byte_patch* allow_wrong_game = nullptr;

GMOD_MODULE_OPEN()
{
	Msg("Steam Emu\n");

	auto addr = ::memory::module("engine").scan(::memory::pattern("48 8B 95 60 FF FF FF 44")).value();

	Msg("Steam Emu found address: %lX\n", (uint64_t)addr.as<void*>());
	allow_invalid_ticket = memory::byte_patch::make(addr.add(46).as<void*>(), {0xE9, 0x16, 0x00, 0x00, 0x00}).get();
	allow_wrong_game = memory::byte_patch::make(addr.sub(27).as<void*>(), {0xE9, 0x5F, 0x00, 0x00, 0x00}).get();

	allow_invalid_ticket->apply();
	allow_wrong_game->apply();
	
	return 0;
}

GMOD_MODULE_CLOSE()
{
	if (allow_invalid_ticket)
		allow_invalid_ticket->restore();
	if (allow_wrong_game)
		allow_wrong_game->restore();

	return 0;
}
