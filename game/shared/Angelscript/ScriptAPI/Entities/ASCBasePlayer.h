#ifndef GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEPLAYER_H
#define GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEPLAYER_H

#include "ASCBaseEntity.h"

/**
*	Class name for CBasePlayer in scripts.
*/
#define AS_CBASEPLAYER_NAME "CBasePlayer"

/**
*	@see RegisterScriptBaseEntity( asIScriptEngine& engine, const char* const pszObjectName )
*/
template<typename CLASS>
void RegisterScriptCBasePlayer( asIScriptEngine& engine, const char* const pszObjectName )
{
	RegisterScriptCBaseEntity<CLASS>( engine, pszObjectName );

	as::RegisterCasts<CBasePlayer, CLASS>( engine, AS_CBASEPLAYER_NAME, pszObjectName, &as::Cast_UpCast, &as::Cast_DownCast );

	//TODO: player interface. - Solokiller
}

inline void RegisterScriptCBasePlayer( asIScriptEngine& engine )
{
	RegisterScriptCBasePlayer<CBasePlayer>( engine, AS_CBASEPLAYER_NAME );
}

#endif //GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEPLAYER_H
