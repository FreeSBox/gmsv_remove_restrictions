// garrysmod_common has pattern scanning, but it's so fucking unreadable and not documented that I'm just gonna take the YimMenu classes instead.
#include "memory/batch.hpp"
#include "memory/pattern.hpp"
#include "memory/module.hpp"

#include "pointers.hpp"

namespace gm
{
	void pointers::init_engine()
	{
		memory::batch batch;

#if ARCHITECTURE_IS_X86_64
		batch.add("Steam Auth", "48 8B 95 60 FF FF FF 44", [this](memory::handle ptr) {
			m_allow_invalid_ticket = ptr.add(46).as<void*>();
			m_allow_wrong_game = ptr.sub(27).as<void*>();
		});

		batch.add("Steam Auth 2", "48 89 C6 4C 89 EF ? ? ? ? ? EB", [this](memory::handle ptr) {
			m_allow_all_invalid_tickets = ptr.add(6).as<void*>();
		});
#elif ARCHITECTURE_IS_X86
		batch.add("Steam Auth", "8B 85 78 FF FF FF C7 04", [this](memory::handle ptr) {
			m_allow_invalid_ticket = ptr.add(0x3F).as<void*>();
			m_allow_wrong_game = ptr.sub(0x1D).as<void*>();
		});

		batch.add("Steam Auth 2", "89 44 24 08 89 5C 24 04 89 3C 24 E8", [this](memory::handle ptr) {
			m_allow_all_invalid_tickets = ptr.add(11).as<void*>();
		});
#endif

		batch.run(memory::module("engine"));
	}

	void pointers::init_server()
	{
		memory::batch batch;

#if ARCHITECTURE_IS_X86_64
		batch.add("ConCommand_IsBlocked", "48 8B 00 80 78 ? ? 75 ? 48 8B 05", [this](memory::handle ptr) {
			m_concommand_is_blocked = ptr.sub(17).as<void*>();
		});
#elif ARCHITECTURE_IS_X86
		batch.add("ConCommand_IsBlocked", "8B 75 08 80 78", [this](memory::handle ptr) {
			m_concommand_is_blocked = ptr.sub(13).as<void*>();
		});
#endif

		batch.run(memory::module("server"));
	}

	pointers::pointers()
	{
		init_engine();

		init_server();

		g_pointers = this;
	}

	pointers::~pointers()
	{
		g_pointers = nullptr;
	}
}