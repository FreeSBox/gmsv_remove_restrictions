#include "byte_patch_manager.hpp"

#include "memory/byte_patch.hpp"
#include "pointers.hpp"

namespace gm
{
	static void init()
	{
#if ARCHITECTURE_IS_X86_64
		memory::byte_patch::make(g_pointers->m_allow_wrong_game, {0xE9, 0x5F, 0x00, 0x00, 0x00})->apply();
#elif ARCHITECTURE_IS_X86
		memory::byte_patch::make(g_pointers->m_allow_wrong_game, {0xE9, 0x74, 0x00, 0x00, 0x00})->apply();
#endif
		memory::byte_patch::make(g_pointers->m_allow_invalid_ticket, {0xE9, 0x18, 0x00, 0x00, 0x00})->apply();

		memory::byte_patch::make(g_pointers->m_allow_all_invalid_tickets, {0x90, 0x90, 0x90, 0x90, 0x90})->apply();
	}

	byte_patch_manager::byte_patch_manager()
	{
		init();

		g_byte_patch_manager = this;
	}

	byte_patch_manager::~byte_patch_manager()
	{
		memory::byte_patch::restore_all();

		g_byte_patch_manager = nullptr;
	}
}
