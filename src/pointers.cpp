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
		memory::module module("engine");

#if ARCHITECTURE_IS_X86_64
		batch.add("Steam Auth", "48 8B 95 60 FF FF FF 44", [this](memory::handle ptr) {
			m_allow_invalid_ticket = ptr.add(46).as<void*>();
			m_allow_wrong_game = ptr.sub(27).as<void*>();
		});

		batch.add("CGameServer", "48 8B 3D ? ? ? ? F3 0F 58", [this](memory::handle ptr) {
			m_sv = ptr.add(3).rip().as<IServer *>();
		});
#elif ARCHITECTURE_IS_X86
		batch.add("Steam Auth", "8B 85 78 FF FF FF C7 04", [this](memory::handle ptr) {
			m_allow_invalid_ticket = ptr.add(0x3F).as<void*>();
			m_allow_wrong_game = ptr.sub(0x1D).as<void*>();
		});

		batch.add("CGameServer", "C7 04 24 ? ? ? ? A3 ? ? ? ? E8 ? ? ? ? D9", [this](memory::handle ptr) {
			m_sv = *ptr.add(3).as<IServer **>();
		});
#endif

		batch.run(module);
	}

	void pointers::init_server()
	{
		memory::batch batch;
		memory::module module("server");

#if ARCHITECTURE_IS_X86_64
		batch.add("ConCommand_IsBlocked", "48 8B 00 80 78 ? ? 75 ? 48 8B 05", [this](memory::handle ptr) {
			m_concommand_is_blocked = ptr.sub(17).as<void*>();
		});
#elif ARCHITECTURE_IS_X86
		batch.add("ConCommand_IsBlocked", "8B 75 08 80 78", [this](memory::handle ptr) {
			m_concommand_is_blocked = ptr.sub(13).as<void*>();
		});
#endif

		batch.run(module);
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