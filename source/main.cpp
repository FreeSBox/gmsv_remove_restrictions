#include "GarrysMod/Lua/Interface.h"
#include "GarrysMod/Symbol.hpp"
#include "GarrysMod/InterfacePointers.hpp"
#include "GarrysMod/FunctionPointers.hpp"
#include "memory/batch.hpp"
#include "scanning/symbolfinder.hpp"

// garrysmod_common has pattern scanning, but it's so fucking unreadable and not documented that I'm just gonna take the YimMenu classes instead.
#include "memory/pattern.hpp"
#include "memory/module.hpp"
#include "memory/byte_patch.hpp"

namespace pointers {
	static void* allow_invalid_ticket{};
	static void* allow_wrong_game{};

	static void* concommand_is_blocked{};

	void init_engine()
	{
		memory::batch batch;

		batch.add("Steam Auth", "48 8B 95 60 FF FF FF 44", [](memory::handle ptr) {
			allow_invalid_ticket = ptr.add(46).as<void*>();
			allow_wrong_game = ptr.sub(27).as<void*>();
		});

		batch.run(memory::module("engine"));
	}

	void init_server()
	{
		memory::batch batch;

		batch.add("ConCommand_IsBlocked", "48 8B 00 80 78 ? ? 75 ? 48 8B 05", [](memory::handle ptr) {
			concommand_is_blocked = ptr.sub(17).as<void*>();
		});

		batch.run(memory::module("server"));
	}
}

namespace byte_patches {
	static memory::byte_patch* allow_invalid_ticket = nullptr;
	static memory::byte_patch* allow_wrong_game = nullptr;
	static memory::byte_patch* allow_blocked_concommand = nullptr;

	void init_patches()
	{
		allow_invalid_ticket = memory::byte_patch::make(pointers::allow_invalid_ticket, {0xE9, 0x16, 0x00, 0x00, 0x00}).get();
		allow_wrong_game = memory::byte_patch::make(pointers::allow_wrong_game, {0xE9, 0x5F, 0x00, 0x00, 0x00}).get();

		
		// Just return 0 here, instead of jumping over in specific functions,
		// because it's impossible to get a pattern there that will work across versions.
		allow_blocked_concommand = memory::byte_patch::make(pointers::concommand_is_blocked, {
			0xB0, 0x00,	// MOV AL, 0
			0xC3		// RET
		}).get();
	}

	void apply_patches()
	{
		allow_invalid_ticket->apply();
		allow_wrong_game->apply();
		allow_blocked_concommand->apply();
	}
}

GMOD_MODULE_OPEN()
{
	pointers::init_engine();
	pointers::init_server();

	byte_patches::init_patches();
	byte_patches::apply_patches();

	Msg("Remove restrictions patches applied.\n");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	memory::byte_patch::restore_all();

	return 0;
}
