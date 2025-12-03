#pragma once

class ILauncherMgr;

namespace gm
{
	class pointers
	{
	public:
		explicit pointers();
		~pointers();

	private:
		void init_engine();
		void init_server();
		void init_launcher();

	public:

		void* m_allow_invalid_ticket{};
		void* m_allow_wrong_game{};
		void* m_allow_all_invalid_tickets{};

		void* m_concommand_is_blocked{};

		ILauncherMgr *m_launcher_mgr{};

		void* m_sdl_poll_event{};
		void* m_sdl_swap_window{};
	};

	inline pointers* g_pointers{};
}
