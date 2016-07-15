#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules/CGameRules.h"

#include "ASGameRules.h"

static std::string CGameRules_GetGameDescription( const CGameRules* pThis )
{
	return pThis->GetGameDescription();
}

/**
*	Class name for CGameRules in scripts.
*/
#define AS_CGAMERULES_CLASSNAME "CGameRules"

void RegisterScriptCGameRules( asIScriptEngine& engine )
{
	const char* const pszObjectName = AS_CGAMERULES_CLASSNAME;

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "void RefreshSkillData()",
		asMETHOD( CGameRules, RefreshSkillData ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Think()",
		asMETHOD( CGameRules, Think ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsAllowedToSpawn(CBaseEntity@ pEntity)",
		asMETHOD( CGameRules, IsAllowedToSpawn ), asCALL_THISCALL );

	//TODO: weapon related methods

	engine.RegisterObjectMethod(
		pszObjectName, "bool FAllowFlashlight() const",
		asMETHOD( CGameRules, FAllowFlashlight ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsMultiplayer() const",
		asMETHOD( CGameRules, IsMultiplayer ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsDeathmatch() const",
		asMETHOD( CGameRules, IsDeathmatch ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsTeamplay() const",
		asMETHOD( CGameRules, IsTeamplay ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsCoOp() const",
		asMETHOD( CGameRules, IsCoOp ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetGameDescription() const",
		asFUNCTION( CGameRules_GetGameDescription ), asCALL_CDECL_OBJFIRST );
}

void RegisterScriptGameRules( asIScriptEngine& engine )
{
	//The base name is just the regular name without the 'C'.
	RegisterScriptGameRules<CGameRules>( engine, AS_CGAMERULES_CLASSNAME + 1 );
}