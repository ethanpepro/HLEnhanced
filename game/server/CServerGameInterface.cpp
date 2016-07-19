#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CBasePlayer.h"
#include "entities/CSoundEnt.h"
#include "entities/CBaseSpectator.h"

#include "gamerules/GameRules.h"
#include "Server.h"

#include "Angelscript/CHLASManager.h"

#include "CServerGameInterface.h"

extern DLL_GLOBAL bool			g_fGameOver;
extern DLL_GLOBAL float			g_flWeaponCheat;
extern DLL_GLOBAL int			g_teamplay;
extern DLL_GLOBAL bool			g_bPrecacheGrunt;
extern DLL_GLOBAL unsigned int	g_ulFrameCount;

extern cvar_t allow_spectators;

void Host_Say( edict_t *pEntity, int teamonly );

CServerGameInterface g_Server;

bool CServerGameInterface::Initialize()
{
	if( !InitializeCommon() )
		return false;

	return true;
}

void CServerGameInterface::Shutdown()
{
	ShutdownCommon();
}

bool CServerGameInterface::ClientConnect( edict_t* pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ CCONNECT_REJECT_REASON_SIZE ] )
{
	return g_pGameRules->ClientConnected( pEntity, pszName, pszAddress, szRejectReason );
}

void CServerGameInterface::ClientDisconnect( edict_t* pEdict )
{
	if( g_fGameOver )
		return;

	//Will be created if it doesn't exist, but should already exist. - Solokiller
	auto pPlayer = GetClassPtr( ( CBasePlayer* ) &pEdict->v );

	char text[ 256 ] = "";
	if( pEdict->v.netname )
		_snprintf( text, sizeof( text ), "- %s has left the game\n", STRING( pEdict->v.netname ) );
	text[ sizeof( text ) - 1 ] = 0;
	MESSAGE_BEGIN( MSG_ALL, gmsgSayText, NULL );
	WRITE_BYTE( ENTINDEX( pEdict ) );
	WRITE_STRING( text );
	MESSAGE_END();

	// since this client isn't around to think anymore, reset their sound. 
	if( auto pSound = CSoundEnt::SoundPointerForIndex( CSoundEnt::ClientSoundIndex( pPlayer ) ) )
	{
		pSound->Reset();
	}

	// since the edict doesn't get deleted, fix it so it doesn't interfere.
	pEdict->v.takedamage = DAMAGE_NO;// don't attract autoaim
	pEdict->v.solid = SOLID_NOT;// nonsolid
	UTIL_SetOrigin( pPlayer, pEdict->v.origin );

	g_pGameRules->ClientDisconnected( pEdict );
}

void CServerGameInterface::ClientKill( edict_t* pEdict )
{
	auto pPlayer = ( CBasePlayer* ) CBasePlayer::Instance( &pEdict->v );

	if( pPlayer->m_fNextSuicideTime > gpGlobals->time )
		return;  // prevent suiciding too often

	pPlayer->m_fNextSuicideTime = gpGlobals->time + 1;  // don't let them suicide for 5 seconds after suiciding

	// have the player kill themself
	pPlayer->pev->health = 0;
	pPlayer->Killed( CTakeDamageInfo( pPlayer, 0, 0 ), GIB_NEVER );
}

void CServerGameInterface::ClientPutInServer( edict_t* pEntity )
{
	// Allocate a CBasePlayer for pev, and call spawn
	auto pPlayer = GetClassPtr( ( CBasePlayer* ) &pEntity->v );

	pPlayer->InitialSpawn();
}

void CServerGameInterface::ClientCommand( edict_t* pEntity )
{
	// Use CMD_ARGV,  CMD_ARGV, and CMD_ARGC to get pointers the character string command.

	const char *pcmd = CMD_ARGV( 0 );
	const char *pstr;

	// Is the client spawned yet?
	if( !pEntity->pvPrivateData )
		return;

	auto* pPlayer = GetClassPtr( ( CBasePlayer* ) &pEntity->v );

	if( FStrEq( pcmd, "say" ) )
	{
		Host_Say( pEntity, 0 );
	}
	else if( FStrEq( pcmd, "say_team" ) )
	{
		Host_Say( pEntity, 1 );
	}
	else if( FStrEq( pcmd, "fullupdate" ) )
	{
		pPlayer->ForceClientDllUpdate();
	}
	else if( FStrEq( pcmd, "give" ) )
	{
		if( g_flWeaponCheat != 0.0 )
		{
			int iszItem = ALLOC_STRING( CMD_ARGV( 1 ) );	// Make a copy of the classname
			pPlayer->GiveNamedItem( STRING( iszItem ) );
		}
	}

	else if( FStrEq( pcmd, "drop" ) )
	{
		// player is dropping an item. 
		pPlayer->DropPlayerItem( ( char * ) CMD_ARGV( 1 ) );
	}
	else if( FStrEq( pcmd, "fov" ) )
	{
		if( g_flWeaponCheat && CMD_ARGC() > 1 )
		{
			pPlayer->m_iFOV = atoi( CMD_ARGV( 1 ) );
		}
		else
		{
			CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "\"fov\" is \"%d\"\n", ( int ) pPlayer->m_iFOV ) );
		}
	}
	else if( FStrEq( pcmd, "use" ) )
	{
		pPlayer->SelectItem( ( char * ) CMD_ARGV( 1 ) );
	}
	else if( ( ( pstr = strstr( pcmd, "weapon_" ) ) != NULL ) && ( pstr == pcmd ) )
	{
		pPlayer->SelectItem( pcmd );
	}
	else if( FStrEq( pcmd, "lastinv" ) )
	{
		pPlayer->SelectLastItem();
	}
	else if( FStrEq( pcmd, "spectate" ) )	// clients wants to become a spectator
	{
		// always allow proxies to become a spectator
		if( ( pPlayer->pev->flags & FL_PROXY ) || allow_spectators.value )
		{
			CBaseEntity *pSpawnSpot = g_pGameRules->GetPlayerSpawnSpot( pPlayer );
			pPlayer->StartObserver( pPlayer->pev->origin, pSpawnSpot->pev->angles );

			// notify other clients of player switching to spectator mode
			UTIL_ClientPrintAll( HUD_PRINTNOTIFY, UTIL_VarArgs( "%s switched to spectator mode\n",
				( pPlayer->pev->netname && STRING( pPlayer->pev->netname )[ 0 ] != 0 ) ? STRING( pPlayer->pev->netname ) : "unconnected" ) );
		}
		else
			ClientPrint( pPlayer->pev, HUD_PRINTCONSOLE, "Spectator mode is disabled.\n" );

	}
	else if( FStrEq( pcmd, "specmode" ) )	// new spectator mode
	{
		if( pPlayer->IsObserver() )
			pPlayer->Observer_SetMode( atoi( CMD_ARGV( 1 ) ) );
	}
	else if( FStrEq( pcmd, "closemenus" ) )
	{
		// just ignore it
	}
	else if( FStrEq( pcmd, "follownext" ) )	// follow next player
	{
		if( pPlayer->IsObserver() )
			pPlayer->Observer_FindNextPlayer( atoi( CMD_ARGV( 1 ) ) != 0 );
	}
	else if( g_pGameRules->ClientCommand( pPlayer, pcmd ) )
	{
		// MenuSelect returns true only if the command is properly handled,  so don't print a warning
	}
	else
	{
		// tell the user they entered an unknown command
		char command[ 128 ];

		// check the length of the command (prevents crash)
		// max total length is 192 ...and we're adding a string below ("Unknown command: %s\n")
		strncpy( command, pcmd, sizeof( command ) - 1 );
		command[ sizeof( command ) - 1 ] = '\0';

		// tell the user they entered an unknown command
		ClientPrint( &pEntity->v, HUD_PRINTCONSOLE, UTIL_VarArgs( "Unknown command: %s\n", command ) );
	}
}

void CServerGameInterface::ClientUserInfoChanged( edict_t* pEntity, char* infobuffer )
{
	// Is the client spawned yet?
	if( !pEntity->pvPrivateData )
		return;

	// msg everyone if someone changes their name,  and it isn't the first time (changing no name to current name)
	if( pEntity->v.netname && STRING( pEntity->v.netname )[ 0 ] != 0 && !FStrEq( STRING( pEntity->v.netname ), g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ) ) )
	{
		char sName[ 256 ];
		char *pName = g_engfuncs.pfnInfoKeyValue( infobuffer, "name" );
		strncpy( sName, pName, sizeof( sName ) - 1 );
		sName[ sizeof( sName ) - 1 ] = '\0';

		// First parse the name and remove any %'s
		for( char *pApersand = sName; pApersand != NULL && *pApersand != 0; pApersand++ )
		{
			// Replace it with a space
			if( *pApersand == '%' )
				*pApersand = ' ';
		}

		// Set the name
		g_engfuncs.pfnSetClientKeyValue( ENTINDEX( pEntity ), infobuffer, "name", sName );

		if( gpGlobals->maxClients > 1 )
		{
			char text[ 256 ];
			sprintf( text, "* %s changed name to %s\n", STRING( pEntity->v.netname ), g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ) );
			MESSAGE_BEGIN( MSG_ALL, gmsgSayText, NULL );
			WRITE_BYTE( ENTINDEX( pEntity ) );
			WRITE_STRING( text );
			MESSAGE_END();
		}

		// team match?
		if( g_teamplay )
		{
			UTIL_LogPrintf( "\"%s<%i><%s><%s>\" changed name to \"%s\"\n",
							STRING( pEntity->v.netname ),
							GETPLAYERUSERID( pEntity ),
							GETPLAYERAUTHID( pEntity ),
							g_engfuncs.pfnInfoKeyValue( infobuffer, "model" ),
							g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ) );
		}
		else
		{
			UTIL_LogPrintf( "\"%s<%i><%s><%i>\" changed name to \"%s\"\n",
							STRING( pEntity->v.netname ),
							GETPLAYERUSERID( pEntity ),
							GETPLAYERAUTHID( pEntity ),
							GETPLAYERUSERID( pEntity ),
							g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ) );
		}
	}

	g_pGameRules->ClientUserInfoChanged( GetClassPtr( ( CBasePlayer * ) &pEntity->v ), infobuffer );
}

void CServerGameInterface::Activate( edict_t* pEdictList, const int edictCount, const int clientMax )
{
	CBaseEntity* pClass;

	// Every call to ServerActivate should be matched by a call to ServerDeactivate
	m_bActive = true;

	// Clients have not been initialized yet
	for( int i = 0; i < edictCount; ++i )
	{
		if( pEdictList[ i ].free )
			continue;

		// Clients aren't necessarily initialized until ClientPutInServer()
		if( i < clientMax || !pEdictList[ i ].pvPrivateData )
			continue;

		pClass = CBaseEntity::Instance( &pEdictList[ i ] );
		// Activate this entity if it's got a class & isn't dormant
		if( pClass && !( pClass->pev->flags & FL_DORMANT ) )
		{
			pClass->Activate();
		}
		else
		{
			ALERT( at_console, "Can't instance %s\n", STRING( pEdictList[ i ].v.classname ) );
		}
	}

	// Link user messages here to make sure first client can get them...
	LinkUserMessages();

	g_ASManager.WorldActivated();
}

void CServerGameInterface::Deactivate()
{
	// It's possible that the engine will call this function more times than is necessary
	//  Therefore, only run it one time for each call to Activate 
	if( !m_bActive )
	{
		return;
	}

	m_bActive = false;

	// Peform any shutdown operations here...
	//
}

void CServerGameInterface::StartFrame()
{
	if( g_pGameRules )
		g_pGameRules->Think();

	if( g_fGameOver )
		return;

	gpGlobals->teamplay = teamplay.value;
	++g_ulFrameCount;
}

void CServerGameInterface::ParmsNewLevel()
{
}

void CServerGameInterface::ParmsChangeLevel()
{
	// retrieve the pointer to the save data
	SAVERESTOREDATA *pSaveData = ( SAVERESTOREDATA * ) gpGlobals->pSaveData;

	if( pSaveData )
		pSaveData->connectionCount = BuildChangeList( pSaveData->levelList, MAX_LEVEL_CONNECTIONS );
}

void CServerGameInterface::ClientPrecache()
{
	// setup precaches always needed
	PRECACHE_SOUND( "player/sprayer.wav" );			// spray paint sound for PreAlpha

	// PRECACHE_SOUND("player/pl_jumpland2.wav");		// UNDONE: play 2x step sound

	PRECACHE_SOUND( "player/pl_fallpain2.wav" );
	PRECACHE_SOUND( "player/pl_fallpain3.wav" );

	PRECACHE_SOUND( "player/pl_step1.wav" );		// walk on concrete
	PRECACHE_SOUND( "player/pl_step2.wav" );
	PRECACHE_SOUND( "player/pl_step3.wav" );
	PRECACHE_SOUND( "player/pl_step4.wav" );

	PRECACHE_SOUND( "common/npc_step1.wav" );		// NPC walk on concrete
	PRECACHE_SOUND( "common/npc_step2.wav" );
	PRECACHE_SOUND( "common/npc_step3.wav" );
	PRECACHE_SOUND( "common/npc_step4.wav" );

	PRECACHE_SOUND( "player/pl_metal1.wav" );		// walk on metal
	PRECACHE_SOUND( "player/pl_metal2.wav" );
	PRECACHE_SOUND( "player/pl_metal3.wav" );
	PRECACHE_SOUND( "player/pl_metal4.wav" );

	PRECACHE_SOUND( "player/pl_dirt1.wav" );		// walk on dirt
	PRECACHE_SOUND( "player/pl_dirt2.wav" );
	PRECACHE_SOUND( "player/pl_dirt3.wav" );
	PRECACHE_SOUND( "player/pl_dirt4.wav" );

	PRECACHE_SOUND( "player/pl_duct1.wav" );		// walk in duct
	PRECACHE_SOUND( "player/pl_duct2.wav" );
	PRECACHE_SOUND( "player/pl_duct3.wav" );
	PRECACHE_SOUND( "player/pl_duct4.wav" );

	PRECACHE_SOUND( "player/pl_grate1.wav" );		// walk on grate
	PRECACHE_SOUND( "player/pl_grate2.wav" );
	PRECACHE_SOUND( "player/pl_grate3.wav" );
	PRECACHE_SOUND( "player/pl_grate4.wav" );

	PRECACHE_SOUND( "player/pl_slosh1.wav" );		// walk in shallow water
	PRECACHE_SOUND( "player/pl_slosh2.wav" );
	PRECACHE_SOUND( "player/pl_slosh3.wav" );
	PRECACHE_SOUND( "player/pl_slosh4.wav" );

	PRECACHE_SOUND( "player/pl_tile1.wav" );		// walk on tile
	PRECACHE_SOUND( "player/pl_tile2.wav" );
	PRECACHE_SOUND( "player/pl_tile3.wav" );
	PRECACHE_SOUND( "player/pl_tile4.wav" );
	PRECACHE_SOUND( "player/pl_tile5.wav" );

	PRECACHE_SOUND( "player/pl_swim1.wav" );		// breathe bubbles
	PRECACHE_SOUND( "player/pl_swim2.wav" );
	PRECACHE_SOUND( "player/pl_swim3.wav" );
	PRECACHE_SOUND( "player/pl_swim4.wav" );

	PRECACHE_SOUND( "player/pl_ladder1.wav" );	// climb ladder rung
	PRECACHE_SOUND( "player/pl_ladder2.wav" );
	PRECACHE_SOUND( "player/pl_ladder3.wav" );
	PRECACHE_SOUND( "player/pl_ladder4.wav" );

	PRECACHE_SOUND( "player/pl_wade1.wav" );		// wade in water
	PRECACHE_SOUND( "player/pl_wade2.wav" );
	PRECACHE_SOUND( "player/pl_wade3.wav" );
	PRECACHE_SOUND( "player/pl_wade4.wav" );

	PRECACHE_SOUND( "debris/wood1.wav" );			// hit wood texture
	PRECACHE_SOUND( "debris/wood2.wav" );
	PRECACHE_SOUND( "debris/wood3.wav" );

	PRECACHE_SOUND( "plats/train_use1.wav" );		// use a train

	PRECACHE_SOUND( "buttons/spark5.wav" );		// hit computer texture
	PRECACHE_SOUND( "buttons/spark6.wav" );
	PRECACHE_SOUND( "debris/glass1.wav" );
	PRECACHE_SOUND( "debris/glass2.wav" );
	PRECACHE_SOUND( "debris/glass3.wav" );

	PRECACHE_SOUND( SOUND_FLASHLIGHT_ON );
	PRECACHE_SOUND( SOUND_FLASHLIGHT_OFF );

	// player gib sounds
	PRECACHE_SOUND( "common/bodysplat.wav" );

	// player pain sounds
	PRECACHE_SOUND( "player/pl_pain2.wav" );
	PRECACHE_SOUND( "player/pl_pain4.wav" );
	PRECACHE_SOUND( "player/pl_pain5.wav" );
	PRECACHE_SOUND( "player/pl_pain6.wav" );
	PRECACHE_SOUND( "player/pl_pain7.wav" );

	PRECACHE_MODEL( "models/player.mdl" );

	// hud sounds

	PRECACHE_SOUND( "common/wpn_hudoff.wav" );
	PRECACHE_SOUND( "common/wpn_hudon.wav" );
	PRECACHE_SOUND( "common/wpn_moveselect.wav" );
	PRECACHE_SOUND( "common/wpn_select.wav" );
	PRECACHE_SOUND( "common/wpn_denyselect.wav" );


	// geiger sounds

	PRECACHE_SOUND( "player/geiger6.wav" );
	PRECACHE_SOUND( "player/geiger5.wav" );
	PRECACHE_SOUND( "player/geiger4.wav" );
	PRECACHE_SOUND( "player/geiger3.wav" );
	PRECACHE_SOUND( "player/geiger2.wav" );
	PRECACHE_SOUND( "player/geiger1.wav" );

	if( g_bPrecacheGrunt )
		UTIL_PrecacheOther( "monster_human_grunt" );
}

const char* CServerGameInterface::GetGameDescription() const
{
	// this function may be called before the world has spawned, and the game rules initialized
	if( g_pGameRules )
		return g_pGameRules->GetGameDescription();
	else
		return "Half-Life";
}

void CServerGameInterface::Sys_Error( const char* error_string )
{
	// Default case, do nothing.  MOD AUTHORS:  Add code ( e.g., _asm { int 3 }; here to cause a breakpoint for debugging your game .dlls
}

void CServerGameInterface::PlayerCustomization( edict_t* pEntity, customization_t* pCust )
{
	/*
	UNDONE:  This only sets the # of frames of the spray can logo
	animation right now.
	*/

	auto pPlayer = ( CBasePlayer* ) GET_PRIVATE( pEntity );

	if( !pPlayer )
	{
		ALERT( at_console, "PlayerCustomization:  Couldn't get player!\n" );
		return;
	}

	if( !pCust )
	{
		ALERT( at_console, "PlayerCustomization:  NULL customization!\n" );
		return;
	}

	switch( pCust->resource.type )
	{
	case t_decal:
		pPlayer->SetCustomDecalFrames( pCust->nUserData2 ); // Second int is max # of frames.
		break;
	case t_sound:
	case t_skin:
	case t_model:
		// Ignore for now.
		break;
	default:
		ALERT( at_console, "PlayerCustomization:  Unknown customization type!\n" );
		break;
	}
}

void CServerGameInterface::SpectatorConnect( edict_t* pEntity )
{
	if( auto pPlayer = ( CBaseSpectator* ) GET_PRIVATE( pEntity ) )
		pPlayer->SpectatorConnect();
}

void CServerGameInterface::SpectatorDisconnect( edict_t* pEntity )
{
	if( auto pPlayer = ( CBaseSpectator* ) GET_PRIVATE( pEntity ) )
		pPlayer->SpectatorDisconnect();
}

void CServerGameInterface::SpectatorThink( edict_t* pEntity )
{
	if( auto pPlayer = ( CBaseSpectator* ) GET_PRIVATE( pEntity ) )
		pPlayer->SpectatorThink();
}