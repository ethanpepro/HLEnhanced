#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASEVENTS_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASEVENTS_H

#include <cstdint>

#include <Angelscript/CASEvent.h>

class asIScriptEngine;

/**
*	@defgroup HLSVEvents Server script events
*	
*	Server side global Angelscript events that can be hooked into.
*
*	@{
*/

extern CASEvent g_ClientPutInServerEvent;

//TODO: these flags are needed to skip specific behavior, it's best to avoid creating the situation that would need such flags to begin with. - Solokiller
namespace PreThinkFlag
{
enum PreThinkFlag : uint32_t
{
	NONE			= 0,
	SKIP_VEHICLES	= 1 << 0,
};
}

/**
*	Called when the player's PreThink is being executed.
*	Parameters:
*	CBasePlayer@ pSelf
*	uint32& out uiFlags
*/
extern CASEvent g_PlayerPreThinkEvent;

namespace UseFlag
{
enum UseFlag : uint32_t
{
	NONE		= 0,
	SKIP_USE	= 1 << 0,
};
}

extern CASEvent g_PlayerUseEvent;

extern CASEvent g_PlayerSayEvent;

/**
*	Registers event data.
*	@param engine Script engine.
*/
void RegisterScriptEventData( asIScriptEngine& engine );

/** @} */

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASEVENTS_H
