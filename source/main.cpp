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
	static void* allow_all_invalid_tickets{};

	static void* concommand_is_blocked{};

	void init_engine()
	{
		memory::batch batch;

#if ARCHITECTURE_IS_X86_64
		batch.add("Steam Auth", "48 8B 95 60 FF FF FF 44", [](memory::handle ptr) {
			allow_invalid_ticket = ptr.add(46).as<void*>();
			allow_wrong_game = ptr.sub(27).as<void*>();
		});

		batch.add("Steam Auth 2", "48 89 C6 4C 89 EF ? ? ? ? ? EB", [](memory::handle ptr) {
			allow_all_invalid_tickets = ptr.add(6).as<void*>();
		});
#elif ARCHITECTURE_IS_X86
		batch.add("Steam Auth", "8D 45 A0 89 44 24 04 8B", [](memory::handle ptr) {
			allow_invalid_ticket = ptr.sub(0x1D).as<void*>();
			allow_wrong_game = ptr.sub(0x73).as<void*>();
		});

		batch.add("Steam Auth 2", "89 44 24 08 89 5C 24 04 89 3C 24 ? ? ? ? ? E9", [](memory::handle ptr) {
			allow_all_invalid_tickets = ptr.add(9).as<void*>();
		});
#endif

		batch.run(memory::module("engine"));
	}

	void init_server()
	{
		memory::batch batch;

#if ARCHITECTURE_IS_X86_64
		batch.add("ConCommand_IsBlocked", "48 8B 00 80 78 ? ? 75 ? 48 8B 05", [](memory::handle ptr) {
			concommand_is_blocked = ptr.sub(17).as<void*>();
		});
#elif ARCHITECTURE_IS_X86
		batch.add("ConCommand_IsBlocked", "8B 75 08 80 78", [](memory::handle ptr) {
			concommand_is_blocked = ptr.sub(13).as<void*>();
		});
#endif

		batch.run(memory::module("server"));
	}
}

namespace byte_patches {
	static memory::byte_patch* allow_invalid_ticket = nullptr;
	static memory::byte_patch* allow_wrong_game = nullptr;
	static memory::byte_patch* allow_all_invalid_tickets = nullptr;

	static memory::byte_patch* allow_blocked_concommand = nullptr;

	void init_patches()
	{
#if ARCHITECTURE_IS_X86_64
		allow_invalid_ticket = memory::byte_patch::make(pointers::allow_invalid_ticket, {0xE9, 0x16, 0x00, 0x00, 0x00}).get();
		allow_wrong_game = memory::byte_patch::make(pointers::allow_wrong_game, {0xE9, 0x5F, 0x00, 0x00, 0x00}).get();
#elif ARCHITECTURE_IS_X86
		allow_invalid_ticket = memory::byte_patch::make(pointers::allow_invalid_ticket, {0xE9, 0x18, 0x00, 0x00, 0x00}).get();
		allow_wrong_game = memory::byte_patch::make(pointers::allow_wrong_game, {0xE9, 0x6E, 0x00, 0x00, 0x00}).get();
#endif

		allow_all_invalid_tickets = memory::byte_patch::make(pointers::allow_all_invalid_tickets, {0x90, 0x90, 0x90, 0x90, 0x90}).get();

		// Just return 0 here, instead of jumping over in specific functions,
		// because it's impossible to get a pattern there that will work across versions.
#if ARCHITECTURE_IS_X86_64
		allow_blocked_concommand = memory::byte_patch::make(pointers::concommand_is_blocked, {
			0x48, 0xC7, 0xC0, 0x00, 0x00, 0x00, 0x00,	// MOV RAX, 0
			0xC3										// RET
		}).get();
#elif ARCHITECTURE_IS_X86
		allow_blocked_concommand = memory::byte_patch::make(pointers::concommand_is_blocked, {
			0xB8, 0x00, 0x00, 0x00, 0x00,	// MOV EAX, 0
			0xC3							// RET
		}).get();

#endif
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
