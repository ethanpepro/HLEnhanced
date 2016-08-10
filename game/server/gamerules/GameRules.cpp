#include "extdll.h"
#include "util.h"
#include "Server.h"

#include "GameRules.h"
#include "CHalfLifeRules.h"
#include "CHalfLifeMultiplay.h"
#include "CHalfLifeTeamplay.h"

#if USE_ANGELSCRIPT
#include "Angelscript/CHLASServerManager.h"
#endif

DLL_GLOBAL CGameRules* g_pGameRules = nullptr;

int g_teamplay = 0;

//=========================================================
// instantiate the proper game rules object
//=========================================================

CGameRules *InstallGameRules()
{
	SERVER_COMMAND( "exec game.cfg\n" );
	SERVER_EXECUTE();

#if USE_ANGELSCRIPT
	//Use the one provided by the script if it exists. - Solokiller
	if( auto pGameRules = g_ASManager.CreateGameRules() )
	{
		return pGameRules;
	}
#endif

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