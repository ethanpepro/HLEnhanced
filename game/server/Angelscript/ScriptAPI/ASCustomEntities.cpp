#include <angelscript.h>

#include "Angelscript/Entities/CASCustomEntities.h"

#include "ASCustomEntities.h"

#define CCUSTOMENTITIES_CLASSNAME "CCustomEntities"

static void RegisterScriptCASCustomEntities( asIScriptEngine& engine )
{
	const char* const pszObjectName = CCUSTOMENTITIES_CLASSNAME;

	engine.RegisterObjectType(
		pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "bool RegisterCustomEntity(const string& in szMapName, const string& in szClassName)",
		asMETHOD( CASCustomEntities, RegisterCustomEntity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool UnregisterCustomEntity(const string& in szMapName, const string& in szClassName)",
		asMETHOD( CASCustomEntities, UnregisterCustomEntity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseEntity@ CreateCustomEntity(const string& in szMapName)",
		asMETHODPR( CASCustomEntities, CreateCustomEntity, ( const std::string& ), CBaseEntity* ), asCALL_THISCALL );

	engine.RegisterGlobalProperty( CCUSTOMENTITIES_CLASSNAME " g_CustomEntities", &g_CustomEntities );
}

static void RegisterScriptICustomEntity( asIScriptEngine& engine )
{
	const char* const pszObjectName = "ICustomEntity";

	engine.RegisterInterface( pszObjectName );
}

void RegisterScriptCustomEntities( asIScriptEngine& engine )
{
	RegisterScriptCASCustomEntities( engine );
	RegisterScriptICustomEntity( engine );
}
