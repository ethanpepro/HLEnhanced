#ifndef GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEPLAYER_H
#define GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEPLAYER_H

#include "ASCBaseEntity.h"

/**
*	Registers constants related to players.
*	@param engine Script engine.
*/
void RegisterScriptPlayerDependencies( asIScriptEngine& engine );

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

	engine.RegisterObjectProperty(
		pszObjectName, "int m_afButtonLast",
		asOFFSET( CLASS, m_afButtonLast ) );

	engine.RegisterObjectProperty(
		pszObjectName, "int m_afButtonPressed",
		asOFFSET( CLASS, m_afButtonPressed ) );

	engine.RegisterObjectProperty(
		pszObjectName, "int m_afButtonReleased",
		asOFFSET( CLASS, m_afButtonReleased ) );

	engine.RegisterObjectProperty(
		pszObjectName, "uint m_afPhysicsFlags",
		asOFFSET( CLASS, m_afPhysicsFlags ) );

	engine.RegisterObjectProperty(
		pszObjectName, "int m_iTrain",
		asOFFSET( CLASS, m_iTrain ) );

	engine.RegisterObjectProperty(
		pszObjectName, "EHANDLE m_hTank",
		asOFFSET( CLASS, m_pTank ) );
}

inline void RegisterScriptCBasePlayer( asIScriptEngine& engine )
{
	RegisterScriptCBasePlayer<CBasePlayer>( engine, AS_CBASEPLAYER_NAME );
}

#endif //GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEPLAYER_H
