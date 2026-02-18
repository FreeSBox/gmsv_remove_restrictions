/**
 * @file hooking.cpp
 * @brief Hooking initialization.
 */

#include "hooking.hpp"
#include "pointers.hpp"

#include <MinHook.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"


namespace gm
{
	hooking::hooking()
	{
		// The only instances in that vector at this point should only be the "lazy" hooks
		// aka the ones that still don't have their m_target assigned
		for (auto& detour_hook_helper : m_detour_hook_helpers)
		{
			detour_hook_helper->m_detour_hook->set_target_and_create_hook(detour_hook_helper->m_on_hooking_available());
		}

		detour_hook_helper::add<hooks::ConCommand_IsBlocked>("ConCommand_IsBlocked", g_pointers->m_concommand_is_blocked);
		detour_hook_helper::add<hooks::CSteam3Server_OnValidateAuthTicketResponse>("CSteam3Server_OnValidateAuthTicketResponse", g_pointers->m_auth_responce);

		g_hooking = this;
	}

	hooking::~hooking()
	{
		if (m_enabled)
			disable();

		g_hooking = nullptr;
	}

	void hooking::enable()
	{
		for (const auto& detour_hook_helper : m_detour_hook_helpers)
		{
			detour_hook_helper->m_detour_hook->enable();
		}

		MH_ApplyQueued();

		m_enabled = true;
	}

	void hooking::disable()
	{
		m_enabled = false;

		for (const auto& detour_hook_helper : m_detour_hook_helpers)
		{
			detour_hook_helper->m_detour_hook->disable();
		}

		MH_ApplyQueued();

		for (const auto& detour_hook_helper : m_detour_hook_helpers)
		{
			delete detour_hook_helper;
		}
		m_detour_hook_helpers.clear();
	}

	hooking::detour_hook_helper::~detour_hook_helper()
	{
		delete m_detour_hook;
	}

	void hooking::detour_hook_helper::enable_hook_if_hooking_is_already_running()
	{
		if (g_hooking && g_hooking->m_enabled)
		{
			if (m_on_hooking_available)
			{
				m_detour_hook->set_target_and_create_hook(m_on_hooking_available());
			}

			m_detour_hook->enable();
			MH_ApplyQueued();
		}
	}
}

#pragma GCC diagnostic pop
