#include "hooking/hooking.hpp"
#include "pointers.hpp"
#include "steam/isteamuser.h"
#include "inetchannel.h"
#include "../engine/baseclient.h"
#include <tier0/dbg.h>

namespace gm
{
	void hooks::CSteam3Server_OnValidateAuthTicketResponse(CSteam3Server* _this, ValidateAuthTicketResponse_t* responce)
	{
		CBaseClient* client = g_pointers->m_find_client_from_steamid(_this, &responce->m_SteamID);
		if (client == nullptr)
		{
			// While the original also just returns if there is no client,
			// it is still nice to call in case it starts doing something
			// or someone else hooked it.
			g_hooking->get_original<hooks::CSteam3Server_OnValidateAuthTicketResponse>()(_this, responce);
			return;
		}

		EAuthSessionResponse orignal_responce = responce->m_eAuthSessionResponse;
		INetChannel* netchan = client->GetNetChannel();

		if (orignal_responce != k_EAuthSessionResponseOK && !(netchan && netchan->IsTimedOut()) )
		{
			responce->m_eAuthSessionResponse = k_EAuthSessionResponseOK;
		}

		g_hooking->get_original<hooks::CSteam3Server_OnValidateAuthTicketResponse>()(_this, responce);

		// All logs must be after the call to the original or the game will crash in find_client_from_steamid when comparing steamids
		// No idea why, don't really want to figure it out.
		if (orignal_responce != responce->m_eAuthSessionResponse)
		{
			Warning("Steam ticket failed to validate (%d), forcing it to k_EAuthSessionResponseOK\n", orignal_responce);
		}
	}
}
