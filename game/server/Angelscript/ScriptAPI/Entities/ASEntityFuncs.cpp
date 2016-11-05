#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ASEntityFuncs.h"

static void RegisterScriptDropToFloor( asIScriptEngine& engine )
{
	const char* const pszObjectName = "DropToFloor";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "STUCK", static_cast<int>( DropToFloor::STUCK ) );
	engine.RegisterEnumValue( pszObjectName, "TOOFAR", static_cast<int>( DropToFloor::TOOFAR ) );
	engine.RegisterEnumValue( pszObjectName, "DROPPED", static_cast<int>( DropToFloor::DROPPED ) );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptWalkMove( asIScriptEngine& engine )
{
	const char* const pszObjectName = "WalkMove";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "NORMAL", WALKMOVE_NORMAL );
	engine.RegisterEnumValue( pszObjectName, "WORLDONLY", WALKMOVE_WORLDONLY );
	engine.RegisterEnumValue( pszObjectName, "CHECKONLY", WALKMOVE_CHECKONLY );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

namespace Entity
{
static CBaseEntity* Create( const std::string& szClassname, const Vector& vecOrigin, const Vector& vecAngles, CBaseEntity* pOwner = nullptr, const bool bSpawnEntity = true )
{
	return CBaseEntity::Create( STRING( ALLOC_STRING( szClassname.c_str() ) ), vecOrigin, vecAngles, pOwner ? pOwner->edict() : nullptr, bSpawnEntity );
}

static CBaseEntity* FindEntityByClassname( CBaseEntity* pStartEntity, const std::string& szClassname )
{
	return UTIL_FindEntityByClassname( pStartEntity, szClassname.c_str() );
}

static CBaseEntity* FindEntityByTargetname( CBaseEntity* pStartEntity, const std::string& szTargetname )
{
	return UTIL_FindEntityByTargetname( pStartEntity, szTargetname.c_str() );
}

static CBaseEntity* FindEntityByTarget( CBaseEntity* pStartEntity, const std::string& szTarget )
{
	return UTIL_FindEntityByTarget( pStartEntity, szTarget.c_str() );
}

static CBaseEntity* FindEntityByString( CBaseEntity* pStartEntity, const std::string& szKeyword, const std::string& szValue )
{
	return UTIL_FindEntityByString( pStartEntity, szKeyword.c_str(), szValue.c_str() );
}

static CBaseEntity* FindEntityGeneric( const std::string& szName, const Vector& vecSrc, const float flRadius )
{
	return UTIL_FindEntityGeneric( szName.c_str(), vecSrc, flRadius );
}

static CBaseEntity* EntityFromIndex( const int iIndex )
{
	if( auto pEntity = g_engfuncs.pfnPEntityOfEntIndex( iIndex ) )
		return GET_PRIVATE( pEntity );

	return nullptr;
}
}

void RegisterScriptEntityFuncs( asIScriptEngine& engine )
{
	RegisterScriptDropToFloor( engine );
	RegisterScriptWalkMove( engine );

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "Entity" );

	engine.RegisterGlobalFunction( 
		"CBaseEntity@ Create(const string& in szClassname, const Vector& in vecOrigin, const Vector& in vecAngles, CBaseEntity@ pOwner = null, const bool bSpawnEntity = true)",
		asFUNCTION( Entity::Create ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindEntityByClassname(CBaseEntity@ pStartEntity, const string& in szClassname)",
		asFUNCTION( Entity::FindEntityByClassname ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindEntityByTargetname(CBaseEntity@ pStartEntity, const string& in szTargetname)",
		asFUNCTION( Entity::FindEntityByTargetname ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindEntityByTarget(CBaseEntity@ pStartEntity, const string& in szTarget)",
		asFUNCTION( Entity::FindEntityByTarget ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindEntityByString(CBaseEntity@ pStartEntity, const string& in szKeyword, const string& in szValue)",
		asFUNCTION( Entity::FindEntityByString ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindEntityInSphere(CBaseEntity@ pStartEntity, const Vector& in vecSrc, const float flRadius)",
		asFUNCTION( UTIL_FindEntityInSphere ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindEntityGeneric(const string& in szName, const Vector& in vecSrc, const float flRadius)",
		asFUNCTION( Entity::FindEntityGeneric ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindEntityForward(CBaseEntity@ pMe)",
		asFUNCTION( UTIL_FindEntityForward ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindEntitiesInPVS(CBaseEntity@ pEntity)",
		asFUNCTION( UTIL_FindEntitiesInPVS ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void Remove(CBaseEntity@ pEntity)",
		asFUNCTION( UTIL_Remove ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void MakeStatic(CBaseEntity@ pEntity)",
		asFUNCTION( UTIL_MakeStatic ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"bool EntIsOnFloor(CBaseEntity@ pEntity)",
		asFUNCTION( UTIL_EntIsOnFloor ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"DropToFloor::DropToFloor DropToFloor(CBaseEntity@ pEntity)",
		asFUNCTION( UTIL_DropToFloor ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"bool WalkMove(CBaseEntity@ pEntity, float flYaw, float flDist, WalkMove::WalkMove mode)",
		asFUNCTION( UTIL_WalkMove ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"CBaseEntity@ EntityFromIndex(const int iIndex)",
		asFUNCTION( Entity::EntityFromIndex ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}
