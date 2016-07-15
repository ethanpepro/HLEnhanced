#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules/CGameRules.h"

#include "ASGameRules.h"

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
}

void RegisterScriptGameRules( asIScriptEngine& engine )
{
	//The base name is just the regular name without the 'C'.
	RegisterScriptGameRules<CGameRules>( engine, AS_CGAMERULES_CLASSNAME + 1 );
}