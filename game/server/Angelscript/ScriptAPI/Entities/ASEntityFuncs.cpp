#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ASEntityFuncs.h"

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
}

void RegisterScriptEntityFuncs( asIScriptEngine& engine )
{
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

	engine.SetDefaultNamespace( szOldNS.c_str() );
}
