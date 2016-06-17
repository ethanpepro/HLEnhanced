#include "extdll.h"
#include "util.h"
#include "game.h"

#include "GameRules.h"
#include "CHalfLifeRules.h"
#include "CHalfLifeMultiplay.h"
#include "CHalfLifeTeamplay.h"

DLL_GLOBAL CGameRules* g_pGameRules = nullptr;

int g_teamplay = 0;

//=========================================================
// instantiate the proper game rules object
//=========================================================

CGameRules *InstallGameRules()
{
	SERVER_COMMAND( "exec game.cfg\n" );
	SERVER_EXECUTE();

	if( !gpGlobals->deathmatch )
	{
		// generic half-life
		g_teamplay = 0;
		return new CHalfLifeRules;
	}
	else
	{
		if( teamplay.value > 0 )
		{
			// teamplay

			g_teamplay = 1;
			return new CHalfLifeTeamplay;
		}
		if( ( int ) gpGlobals->deathmatch == 1 )
		{
			// vanilla deathmatch
			g_teamplay = 0;
			return new CHalfLifeMultiplay;
		}
		else
		{
			// vanilla deathmatch??
			g_teamplay = 0;
			return new CHalfLifeMultiplay;
		}
	}
}