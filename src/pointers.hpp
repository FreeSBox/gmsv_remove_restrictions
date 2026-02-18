#pragma once

class IServer;
class CBaseClient;
class CSteamID;
class CSteam3Server;

namespace gm
{
	using ClientFindFromSteamID = CBaseClient* (*)(CSteam3Server* _this, CSteamID* steamid);

	class pointers
	{
	public:
		explicit pointers();
		~pointers();

	private:
		void init_engine();
		void init_server();

	public:

		void* m_allow_invalid_ticket{};
		void* m_allow_wrong_game{};
		void* m_auth_responce{};
		ClientFindFromSteamID m_find_client_from_steamid{};

		void* m_concommand_is_blocked{};

		IServer* m_sv{};
	};

	inline pointers* g_pointers{};
}
