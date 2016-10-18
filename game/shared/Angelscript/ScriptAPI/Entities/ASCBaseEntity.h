#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEENTITY_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEENTITY_H

#include <string>

#include <angelscript.h>

/**
*	Registers types, constants, globals, etc used by entities.
*	@param engine Script engine.
*/
void RegisterScriptEntityDependencies( asIScriptEngine& engine );

/**
*	Class name for CBaseEntity in scripts.
*/
#define AS_CBASEENTITY_NAME "CBaseEntity"

/**
*	Wrapper so scripts can call KeyValue.
*/
template<typename CLASS>
bool CBaseEntity_KeyValue( CLASS* pThis, const std::string& szKeyName, const std::string& szValue )
{
	KeyValueData kvd;

	kvd.szKeyName		= szKeyName.c_str();
	kvd.szValue			= szValue.c_str();
	kvd.szClassName		= pThis->GetClassname();
	kvd.fHandled		= false;

	pThis->KeyValue( &kvd );

	return kvd.fHandled != 0;
}

std::string CBaseEntity_GetClassname( const CBaseEntity* pThis );

bool CBaseEntity_ClassnameIs( const CBaseEntity* pThis, const std::string& szClassname );

std::string CBaseEntity_GetGlobalName( const CBaseEntity* pThis );

void CBaseEntity_SetGlobalName( CBaseEntity* pThis, const std::string& szGlobalName );

std::string CBaseEntity_GetTargetname( const CBaseEntity* pThis );

void CBaseEntity_SetTargetname( CBaseEntity* pThis, const std::string& szTargetname );

std::string CBaseEntity_GetTarget( const CBaseEntity* pThis );

void CBaseEntity_SetTarget( CBaseEntity* pThis, const std::string& szTarget );

std::string CBaseEntity_GetNetName( const CBaseEntity* pThis );

void CBaseEntity_SetNetName( CBaseEntity* pThis, const std::string& szNetName );

/**
*	Registers CBaseEntity methods and properties.
*	Uses templates to avoid virtual function calls in scripts whenever possible.
*	@param engine Script engine.
*/
template<typename CLASS>
inline void RegisterScriptCBaseEntity( asIScriptEngine& engine, const char* const pszObjectName )
{
	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectProperty(
		pszObjectName, "CBaseEntity@ m_pGoalEnt",
		asOFFSET( CLASS, m_pGoalEnt ) );

	engine.RegisterObjectProperty(
		pszObjectName, "CBaseEntity@ m_pLink",
		asOFFSET( CLASS, m_pLink ) );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetClassname() const",
		asFUNCTION( CBaseEntity_GetClassname ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "bool ClassnameIs(const string& in szClassname) const",
		asFUNCTION( CBaseEntity_ClassnameIs ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "bool HasGlobalName() const",
		asMETHOD( CLASS, HasGlobalName ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetGlobalName() const",
		asFUNCTION( CBaseEntity_GetGlobalName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetGlobalName(const string& in szGlobalName)",
		asFUNCTION( CBaseEntity_SetGlobalName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearGlobalName()",
		asMETHOD( CLASS, ClearGlobalName ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool HasTargetname() const",
		asMETHOD( CLASS, HasTargetname ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetTargetname() const",
		asFUNCTION( CBaseEntity_GetTargetname ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetTargetname(const string& in szTargetname)",
		asFUNCTION( CBaseEntity_SetTargetname ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearTargetname()",
		asMETHOD( CLASS, ClearTargetname ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool HasTarget() const",
		asMETHODPR( CLASS, HasTarget, () const, bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetTarget() const",
		asFUNCTION( CBaseEntity_GetTarget ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetTarget(const string& in szTarget)",
		asFUNCTION( CBaseEntity_SetTarget ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearTarget()",
		asMETHOD( CLASS, ClearTarget ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool HasNetName() const",
		asMETHOD( CLASS, HasNetName ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetNetName() const",
		asFUNCTION( CBaseEntity_GetNetName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetNetName(const string& in szNetName)",
		asFUNCTION( CBaseEntity_SetNetName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearNetName()",
		asMETHOD( CLASS, ClearNetName ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void KeyValue(KeyValueData@ pkvd)",
		asMETHOD( CLASS, KeyValue ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool KeyValue(const string& in szKeyName, const string& in szValue)",
		asFUNCTION( CBaseEntity_KeyValue<CLASS> ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int ObjectCaps() const",
		asMETHOD( CLASS, ObjectCaps ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetObjectCollisionBox()",
		asMETHOD( CLASS, SetObjectCollisionBox ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseEntity@ Respawn()",
		asMETHOD( CLASS, Respawn ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Think()",
		asMETHOD( CLASS, Think ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Touch(CBaseEntity@ pOther)",
		asMETHOD( CLASS, Touch ), asCALL_THISCALL );

	//Give flValue a default so people don't always have to type it.
	engine.RegisterObjectMethod(
		pszObjectName, "void Use(CBaseEntity@ pActivator, CBaseEntity@ pCaller, USE_TYPE useType, float flValue = 0)",
		asMETHOD( CLASS, Use ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Blocked(CBaseEntity@ pOther)",
		asMETHOD( CLASS, Blocked ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int Classify()",
		asMETHOD( CLASS, Classify ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int BloodColor() const",
		asMETHOD( CLASS, BloodColor ), asCALL_THISCALL );
}

inline void RegisterScriptCBaseEntity( asIScriptEngine& engine )
{
	RegisterScriptCBaseEntity<CBaseEntity>( engine, AS_CBASEENTITY_NAME );
}

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEENTITY_H
