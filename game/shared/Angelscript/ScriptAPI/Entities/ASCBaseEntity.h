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

std::string CBaseEntity_GetModelName( const CBaseEntity* pThis );

void CBaseEntity_SetModel( CBaseEntity* pThis, const std::string& szModelName );

std::string CBaseEntity_GetViewModelName( const CBaseEntity* pThis );

void CBaseEntity_SetViewModelName( CBaseEntity* pThis, const std::string& szViewModelName );

std::string CBaseEntity_GetWeaponModelName( const CBaseEntity* pThis );

void CBaseEntity_SetWeaponModelName( CBaseEntity* pThis, const std::string& szWeaponModelName );

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
		pszObjectName, "const Vector& GetAbsOrigin() const",
		asMETHOD( CLASS, GetAbsOrigin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetAbsOrigin(const Vector& in vecOrigin)",
		asMETHOD( CLASS, SetAbsOrigin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetOldOrigin() const",
		asMETHOD( CLASS, GetOldOrigin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetOldOrigin(const Vector& in vecOrigin)",
		asMETHOD( CLASS, SetOldOrigin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetAbsVelocity() const",
		asMETHOD( CLASS, GetAbsVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetAbsVelocity(const Vector& in vecVelocity)",
		asMETHOD( CLASS, SetAbsVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetBaseVelocity() const",
		asMETHOD( CLASS, GetBaseVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetBaseVelocity(const Vector& in vecVelocity)",
		asMETHOD( CLASS, SetBaseVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetMoveDir() const",
		asMETHOD( CLASS, GetMoveDir ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetMoveDir(const Vector& in vecMoveDir)",
		asMETHOD( CLASS, SetMoveDir ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetAbsAngles() const",
		asMETHOD( CLASS, GetAbsAngles ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetAbsAngles(const Vector& in vecAngles)",
		asMETHOD( CLASS, SetAbsAngles ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetAngularVelocity() const",
		asMETHOD( CLASS, GetAngularVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector& GetMutableAVelocity()",
		asMETHOD( CLASS, GetMutableAVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetAngularVelocity(const Vector& in vecVelocity)",
		asMETHOD( CLASS, SetAngularVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetPunchAngle() const",
		asMETHOD( CLASS, GetPunchAngle ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector& GetMutablePunchAngle()",
		asMETHOD( CLASS, GetMutablePunchAngle ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetPunchAngle(const Vector& in vecPunchAngle)",
		asMETHOD( CLASS, SetPunchAngle ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetViewAngle() const",
		asMETHOD( CLASS, GetViewAngle ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector& GetMutableViewAngle()",
		asMETHOD( CLASS, GetMutableViewAngle ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetViewAngle(const Vector& in vecViewAngle)",
		asMETHOD( CLASS, SetViewAngle ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "FixAngleMode GetFixAngleMode() const",
		asMETHOD( CLASS, GetFixAngleMode ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetFixAngleMode(const FixAngleMode mode)",
		asMETHOD( CLASS, SetFixAngleMode ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetIdealPitch() const",
		asMETHOD( CLASS, GetIdealPitch ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetIdealPitch(const float flIdealPitch)",
		asMETHOD( CLASS, SetIdealPitch ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetPitchSpeed() const",
		asMETHOD( CLASS, GetPitchSpeed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetPitchSpeed(const float flPitchSpeed)",
		asMETHOD( CLASS, SetPitchSpeed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetIdealYaw() const",
		asMETHOD( CLASS, GetIdealYaw ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetIdealYaw(const float flIdealYaw)",
		asMETHOD( CLASS, SetIdealYaw ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetYawSpeed() const",
		asMETHOD( CLASS, GetYawSpeed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetYawSpeed(const float flYawSpeed)",
		asMETHOD( CLASS, SetYawSpeed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetModelIndex() const",
		asMETHOD( CLASS, GetModelIndex ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetModelIndex(const int iModelIndex)",
		asMETHOD( CLASS, SetModelIndex ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool HasModel() const",
		asMETHOD( CLASS, HasModel ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetModelName() const",
		asMETHOD( CLASS, GetModelName ), asCALL_THISCALL );

	//Not exposed: SetModelName. The engine handles it, if it's needed add a special handler for it. - Solokiller

	engine.RegisterObjectMethod(
		pszObjectName, "void SetModel(const string& in szModelName)",
		asFUNCTION( CBaseEntity_SetModel ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearModel()",
		asMETHOD( CLASS, ClearModel ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetViewModelName() const",
		asFUNCTION( CBaseEntity_GetViewModelName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetViewModelName(const string& in szViewModelName)",
		asFUNCTION( CBaseEntity_SetViewModelName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearViewModelName()",
		asMETHOD( CLASS, ClearViewModelName ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetWeaponModelName() const",
		asFUNCTION( CBaseEntity_GetWeaponModelName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetWeaponModelName(const string& in szWeaponModelName)",
		asFUNCTION( CBaseEntity_SetWeaponModelName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearWeaponModelName()",
		asMETHOD( CLASS, ClearWeaponModelName ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetWeaponAnim() const",
		asMETHOD( CLASS, GetWeaponAnim ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetWeaponAnim(const int iWeaponAnim)",
		asMETHOD( CLASS, SetWeaponAnim ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsDucking() const",
		asMETHOD( CLASS, IsDucking ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetDucking(const bool bDucking)",
		asMETHOD( CLASS, SetDucking ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetStepSoundTime() const",
		asMETHOD( CLASS, GetStepSoundTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetStepSoundTime(const int iTime)",
		asMETHOD( CLASS, SetStepSoundTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetSwimSoundTime() const",
		asMETHOD( CLASS, GetSwimSoundTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetSwimSoundTime(const int iTime)",
		asMETHOD( CLASS, SetSwimSoundTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetDuckSoundTime() const",
		asMETHOD( CLASS, GetDuckSoundTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetDuckSoundTime(const int iTime)",
		asMETHOD( CLASS, SetDuckSoundTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsStepLeft() const",
		asMETHOD( CLASS, IsStepLeft ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetStepLeft(const bool bStepLeft)",
		asMETHOD( CLASS, SetStepLeft ), asCALL_THISCALL );

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
