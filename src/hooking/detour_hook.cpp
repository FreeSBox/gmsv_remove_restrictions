#include "detour_hook.hpp"

#include "memory/handle.hpp"

#include <MinHook.h>
#include <tier0/dbg.h>

namespace gm
{
	detour_hook::detour_hook(std::string name, void* detour) :
	    m_name(std::move(name)),
	    m_detour(detour)
	{
	}

	detour_hook::detour_hook(std::string name, void* target, void* detour) :
	    m_name(std::move(name)),
	    m_target(target),
	    m_detour(detour)
	{
		create_hook();
	}

	void detour_hook::set_target_and_create_hook(void* target)
	{
		m_target = target;
		create_hook();
	}

	void detour_hook::create_hook()
	{
		fix_hook_address();

		if (auto status = MH_CreateHook(m_target, m_detour, &m_original); status == MH_OK)
		{
			Msg("Created hook '%s'.\n", m_name.c_str());
		}
		else
		{
			Error("Failed to create hook '%s' at 0x%p (error: %s)\n", m_name.c_str(), m_target, MH_StatusToString(status));
		}
	}

	detour_hook::~detour_hook() noexcept
	{
		if (m_target)
		{
			MH_RemoveHook(m_target);
		}

		Msg("Removed hook '%s'.\n", m_name.c_str());
	}

	void detour_hook::enable()
	{
		if (auto status = MH_QueueEnableHook(m_target); status != MH_OK)
			Error("Failed to enable hook 0x%p (%s)\n", m_target, MH_StatusToString(status));
	}

	void detour_hook::disable()
	{
		if (auto status = MH_QueueDisableHook(m_target); status != MH_OK)
			Warning("Failed to disable hook '%s'.\n", m_name.c_str());
	}

	void detour_hook::fix_hook_address()
	{
		auto ptr = memory::handle(m_target);
		while (ptr.as<std::uint8_t&>() == 0xE9)
			ptr = ptr.add(1).rip();
		m_target = ptr.as<void*>();
	}
}
