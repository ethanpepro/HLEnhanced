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
std::string CBaseEntity_GetTargetname( const CBaseEntity* pThis );
std::string CBaseEntity_GetTarget( const CBaseEntity* pThis );

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
		pszObjectName, "string GetClassname() const",
		asFUNCTION( CBaseEntity_GetClassname ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetTargetname() const",
		asFUNCTION( CBaseEntity_GetTargetname ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetTarget() const",
		asFUNCTION( CBaseEntity_GetTarget ), asCALL_CDECL_OBJFIRST );

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