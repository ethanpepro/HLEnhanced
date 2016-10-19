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

std::string CBaseEntity_GetMessage( const CBaseEntity* pThis );

void CBaseEntity_SetMessage( CBaseEntity* pThis, const std::string& szMessage );

bool CBaseEntity_HasTarget( const CBaseEntity* pThis, const std::string& szTarget );

std::string CBaseEntity_TeamID( const CBaseEntity* pThis );

/**
*	Registers CBaseEntity methods and properties.
*	Uses templates to avoid virtual function calls in scripts whenever possible.
*	@param engine Script engine.
*/
template<typename CLASS>
inline void RegisterScriptCBaseEntity( asIScriptEngine& engine, const char* const pszObjectName )
{
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
		pszObjectName, "float GetFallVelocity() const",
		asMETHOD( CLASS, GetFallVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetFallVelocity(const float flFallVelocity)",
		asMETHOD( CLASS, SetFallVelocity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetAbsMin() const",
		asMETHOD( CLASS, GetAbsMin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetAbsMin(const Vector& in vecMin)",
		asMETHOD( CLASS, SetAbsMin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetAbsMax() const",
		asMETHOD( CLASS, GetAbsMax ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetAbsMax(const Vector& in vecMax)",
		asMETHOD( CLASS, SetAbsMax ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetRelMin() const",
		asMETHOD( CLASS, GetRelMin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetRelMin(const Vector& in vecMin)",
		asMETHOD( CLASS, SetRelMin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetRelMax() const",
		asMETHOD( CLASS, GetRelMax ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetRelMax(const Vector& in vecMax)",
		asMETHOD( CLASS, SetRelMax ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetBounds() const",
		asMETHOD( CLASS, GetBounds ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetSize(const Vector& in vecSize)",
		asMETHODPR( CLASS, SetSize, ( const Vector& ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetSize(const Vector& in vecMin, const Vector& in vecMax)",
		asMETHODPR( CLASS, SetSize, ( const Vector&, const Vector& ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetLastThink() const",
		asMETHOD( CLASS, GetLastThink ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetLastThink(const float flLastThink)",
		asMETHOD( CLASS, SetLastThink ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetNextThink() const",
		asMETHOD( CLASS, GetNextThink ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetNextThink(const float flNextThink)",
		asMETHOD( CLASS, SetNextThink ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "MoveType GetMoveType() const",
		asMETHOD( CLASS, GetMoveType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetMoveType(const MoveType moveType)",
		asMETHOD( CLASS, SetMoveType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Solid GetSolidType() const",
		asMETHOD( CLASS, GetSolidType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetSolidType(const Solid solidType)",
		asMETHOD( CLASS, SetSolidType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetSkin() const",
		asMETHOD( CLASS, GetSkin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetSkin(const int iSkin)",
		asMETHOD( CLASS, SetSkin ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetBody() const",
		asMETHOD( CLASS, GetBody ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetBody(const int iBody)",
		asMETHOD( CLASS, SetBody ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const CEntBitSet& GetEffects() const",
		asMETHODPR( CLASS, GetEffects, () const, const CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& GetEffects()",
		asMETHODPR( CLASS, GetEffects, (), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetGravity() const",
		asMETHOD( CLASS, GetGravity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float& GetMutableGravity()",
		asMETHOD( CLASS, GetMutableGravity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetGravity(const float flGravity)",
		asMETHOD( CLASS, SetGravity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetFriction() const",
		asMETHOD( CLASS, GetFriction ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float& GetMutableFriction()",
		asMETHOD( CLASS, GetMutableFriction ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetFriction(const float flFriction)",
		asMETHOD( CLASS, SetFriction ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetLightLevel() const",
		asMETHOD( CLASS, GetLightLevel ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetSequence() const",
		asMETHOD( CLASS, GetSequence ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetSequence(const int iSequence)",
		asMETHOD( CLASS, SetSequence ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetGaitSequence() const",
		asMETHOD( CLASS, GetGaitSequence ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetGaitSequence(const int iGaitSequence)",
		asMETHOD( CLASS, SetGaitSequence ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetFrame() const",
		asMETHOD( CLASS, GetFrame ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float& GetMutableFrame()",
		asMETHOD( CLASS, GetMutableFrame ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetFrame(const float flFrame)",
		asMETHOD( CLASS, SetFrame ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetAnimTime() const",
		asMETHOD( CLASS, GetAnimTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetAnimTime(const float flAnimTime)",
		asMETHOD( CLASS, SetAnimTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetFrameRate() const",
		asMETHOD( CLASS, GetFrameRate ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetFrameRate(const float flFrameRate)",
		asMETHOD( CLASS, SetFrameRate ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetScale() const",
		asMETHOD( CLASS, GetScale ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetScale(const float flScale)",
		asMETHOD( CLASS, SetScale ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "RenderMode GetRenderMode() const",
		asMETHOD( CLASS, GetRenderMode ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetRenderMode(const RenderMode renderMode)",
		asMETHOD( CLASS, SetRenderMode ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetRenderAmount() const",
		asMETHOD( CLASS, GetRenderAmount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetRenderAmount(const float flRenderAmount)",
		asMETHOD( CLASS, SetRenderAmount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetRenderColor() const",
		asMETHOD( CLASS, GetRenderColor ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector& GetMutableRenderColor()",
		asMETHOD( CLASS, GetMutableRenderColor ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetRenderColor(const Vector& in vecColor)",
		asMETHOD( CLASS, SetRenderColor ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "RenderFX GetRenderFX() const",
		asMETHOD( CLASS, GetRenderFX ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetRenderFX(const RenderFX renderFX)",
		asMETHOD( CLASS, SetRenderFX ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetHealth() const",
		asMETHOD( CLASS, GetHealth ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetHealth(const float flHealth)",
		asMETHOD( CLASS, SetHealth ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetMaxHealth() const",
		asMETHOD( CLASS, GetMaxHealth ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetMaxHealth(const float flMaxHealth)",
		asMETHOD( CLASS, SetMaxHealth ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetArmorAmount() const",
		asMETHOD( CLASS, GetArmorAmount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetArmorAmount(const float flArmorAmount)",
		asMETHOD( CLASS, SetArmorAmount ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetArmorType() const",
		asMETHOD( CLASS, GetArmorType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetArmorType(const float flArmorType)",
		asMETHOD( CLASS, SetArmorType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetFrags() const",
		asMETHOD( CLASS, GetFrags ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetFrags(const float flFrags)",
		asMETHOD( CLASS, SetFrags ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const CEntBitSet& GetWeapons() const",
		asMETHODPR( CLASS, GetWeapons, () const, const CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& GetWeapons()",
		asMETHODPR( CLASS, GetWeapons, (), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "TakeDamageMode GetTakeDamageMode() const",
		asMETHOD( CLASS, GetTakeDamageMode ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetTakeDamageMode(const TakeDamageMode takeDamageMode)",
		asMETHOD( CLASS, SetTakeDamageMode ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "DeadFlag GetDeadFlag() const",
		asMETHOD( CLASS, GetDeadFlag ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetDeadFlag(const DeadFlag deadFlag)",
		asMETHOD( CLASS, SetDeadFlag ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const Vector& GetViewOffset() const",
		asMETHOD( CLASS, GetViewOffset ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector& GetMutableViewOffset()",
		asMETHOD( CLASS, GetMutableViewOffset ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetViewOffset(const Vector& in vecViewOffset)",
		asMETHOD( CLASS, SetViewOffset ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const CEntBitSet& GetButtons() const",
		asMETHODPR( CLASS, GetButtons, () const, const CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& GetButtons()",
		asMETHODPR( CLASS, GetButtons, (), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const CEntBitSet& GetOldButtons() const",
		asMETHODPR( CLASS, GetOldButtons, () const, const CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& GetOldButtons()",
		asMETHODPR( CLASS, GetOldButtons, (), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const CEntBitSet& GetImpulse() const",
		asMETHODPR( CLASS, GetImpulse, () const, const CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& GetImpulse()",
		asMETHODPR( CLASS, GetImpulse, (), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const CEntBitSet& GetSpawnFlags() const",
		asMETHODPR( CLASS, GetSpawnFlags, () const, const CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& GetSpawnFlags()",
		asMETHODPR( CLASS, GetSpawnFlags, (), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const CEntBitSet& GetFlags() const",
		asMETHODPR( CLASS, GetFlags, () const, const CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& GetFlags()",
		asMETHODPR( CLASS, GetFlags, (), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetColorMap() const",
		asMETHODPR( CLASS, GetColorMap, () const, int ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetColorMap(const int iColorMap)",
		asMETHODPR( CLASS, SetColorMap, ( const int ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void GetColorMap( int& out iTopColor, int& out iBottomColor ) const",
		asMETHODPR( CLASS, GetColorMap, ( int&, int& ) const, void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetColorMap(const int iTopColor, const int iBottomColor)",
		asMETHODPR( CLASS, SetColorMap, ( const int, const int ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetTeamID() const",
		asMETHOD( CLASS, GetTeamID ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetTeamID(const int iTeamID)",
		asMETHOD( CLASS, SetTeamID ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetPlayerClass() const",
		asMETHOD( CLASS, GetPlayerClass ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetPlayerClass(const int iPlayerClass)",
		asMETHOD( CLASS, SetPlayerClass ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "WaterLevel GetWaterLevel() const",
		asMETHOD( CLASS, GetWaterLevel ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetWaterLevel(const WaterLevel waterLevel)",
		asMETHOD( CLASS, SetWaterLevel ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Contents GetWaterType() const",
		asMETHOD( CLASS, GetWaterType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetWaterType(const Contents waterType)",
		asMETHOD( CLASS, SetWaterType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool HasMessage() const",
		asMETHOD( CLASS, HasMessage ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetMessage() const",
		asFUNCTION( CBaseEntity_GetMessage ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetMessage(const string& in szMessage)",
		asFUNCTION( CBaseEntity_SetMessage ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearMessage()",
		asMETHOD( CLASS, ClearMessage ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetSpeed() const",
		asMETHOD( CLASS, GetSpeed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetSpeed(const float flSpeed)",
		asMETHOD( CLASS, SetSpeed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetMaxSpeed() const",
		asMETHOD( CLASS, GetMaxSpeed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetMaxSpeed(const float flMaxSpeed)",
		asMETHOD( CLASS, SetMaxSpeed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetAirFinishedTime() const",
		asMETHOD( CLASS, GetAirFinishedTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetAirFinishedTime(const float flime)",
		asMETHOD( CLASS, SetAirFinishedTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetPainFinishedTime() const",
		asMETHOD( CLASS, GetPainFinishedTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetPainFinishedTime(const float flime)",
		asMETHOD( CLASS, SetPainFinishedTime ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetFOV() const",
		asMETHOD( CLASS, GetFOV ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetFOV(const float flFOV)",
		asMETHOD( CLASS, SetFOV ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GetDamage() const",
		asMETHOD( CLASS, GetDamage ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetDamage(const float flDamage)",
		asMETHOD( CLASS, SetDamage ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseEntity@ GetOwner() const",
		asMETHOD( CLASS, GetOwner ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetOwner(CBaseEntity@ pOwner)",
		asMETHOD( CLASS, SetOwner ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseEntity@ GetGroundEntity() const",
		asMETHOD( CLASS, GetGroundEntity ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetGroundEntity(CBaseEntity@ pGroundEntity)",
		asMETHOD( CLASS, SetGroundEntity ), asCALL_THISCALL );

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
		pszObjectName, "int entindex() const",
		asMETHOD( CLASS, entindex ), asCALL_THISCALL );

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

	//Pointers and references are equivalent, so we don't need to update CBaseEntity's methods just yet. - Solokiller
	engine.RegisterObjectMethod(
		pszObjectName, "void TraceAttack(const CTakeDamageInfo& in info, Vector vecDir, TraceResult& in tr)",
		asMETHOD( CLASS, TraceAttack ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void TraceBleed(const CTakeDamageInfo& in info, Vector vecDir, TraceResult& in tr)",
		asMETHOD( CLASS, TraceBleed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void TakeDamage(const CTakeDamageInfo& in info)",
		asMETHODPR( CLASS, TakeDamage, ( const CTakeDamageInfo& ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void TakeDamage(CBaseEntity@ pInflictor, CBaseEntity@ pAttacker, float flDamage, int bitsDamageType)",
		asMETHODPR( CLASS, TakeDamage, ( CBaseEntity*, CBaseEntity*, float, int ), void ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Killed(const CTakeDamageInfo& in info, GibAction gibAction)",
		asMETHOD( CLASS, Killed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "float GiveHealth(float flHealth, int bitsDamageType)",
		asMETHOD( CLASS, GiveHealth ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsTriggered(const CBaseEntity@ pActivator) const",
		asMETHOD( CLASS, IsTriggered ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseMonster@ MyMonsterPointer()",
		asMETHOD( CLASS, MyMonsterPointer ), asCALL_THISCALL );

	//TODO: MySquadMonsterPointer probably won't exist for much longer so don't expose it. - Solokiller

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsMoving() const",
		asMETHOD( CLASS, IsMoving ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void OverrideReset()",
		asMETHOD( CLASS, OverrideReset ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int DamageDecal(int bitsDamageType) const",
		asMETHOD( CLASS, DamageDecal ), asCALL_THISCALL );

	//TODO: OnControls may be replaced with a better way so don't expose it. - Solokiller

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsAlive() const",
		asMETHOD( CLASS, IsAlive ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsBSPModel() const",
		asMETHOD( CLASS, IsBSPModel ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool ReflectGauss() const",
		asMETHOD( CLASS, ReflectGauss ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool HasTarget(const string& in szTarget) const",
		asFUNCTION( CBaseEntity_HasTarget ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsInWorld() const",
		asMETHOD( CLASS, IsInWorld ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsPlayer() const",
		asMETHOD( CLASS, IsPlayer ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsNetClient() const",
		asMETHOD( CLASS, IsNetClient ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseEntity@ GetNextTarget()",
		asMETHOD( CLASS, GetNextTarget ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SUB_Remove()",
		asMETHOD( CLASS, SUB_Remove ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SUB_DoNothing()",
		asMETHOD( CLASS, SUB_DoNothing ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SUB_StartFadeOut()",
		asMETHOD( CLASS, SUB_StartFadeOut ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SUB_FadeOut()",
		asMETHOD( CLASS, SUB_FadeOut ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SUB_CallUseToggle()",
		asMETHOD( CLASS, SUB_CallUseToggle ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool ShouldToggle(USE_TYPE useType, const bool bCurrentState) const",
		asMETHOD( CLASS, ShouldToggle ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void FireBullets("
		"const uint uiShots,"
		"Vector vecSrc, Vector vecDirShooting, Vector vecSpread,"
		"float flDistance, int iBulletType,"
		"int iTracerFreq = 4, int iDamage = 0, CBaseEntity@ pAttacker = null)",
		asMETHOD( CLASS, FireBullets ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void FireBulletsPlayer("
		"const uint uiShots,"
		"Vector vecSrc, Vector vecDirShooting, Vector vecSpread,"
		"float flDistance, int iBulletType,"
		"int iTracerFreq = 4, int iDamage = 0, CBaseEntity@ pAttacker = null, int shared_rand = 0)",
		asMETHOD( CLASS, FireBulletsPlayer ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void SUB_UseTargets(CBaseEntity@ pActivator, USE_TYPE useType, float flValue)",
		asMETHOD( CLASS, SUB_UseTargets ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Intersects(CBaseEntity@ pOther) const",
		asMETHOD( CLASS, Intersects ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void MakeDormant()",
		asMETHOD( CLASS, MakeDormant ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsDormant() const",
		asMETHOD( CLASS, IsDormant ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsLockedByMaster() const",
		asMETHOD( CLASS, IsLockedByMaster ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void DeathNotice(CBaseEntity@ pChild)",
		asMETHOD( CLASS, DeathNotice ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool BarnacleVictimGrabbed(CBaseEntity@ pBarnacle)",
		asMETHOD( CLASS, BarnacleVictimGrabbed ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector Center() const",
		asMETHOD( CLASS, Center ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector EyePosition() const",
		asMETHOD( CLASS, EyePosition ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector EarPosition() const",
		asMETHOD( CLASS, EarPosition ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "Vector BodyTarget(const Vector& in vecPosSrc) const",
		asMETHOD( CLASS, Center ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int Illumination() const",
		asMETHOD( CLASS, Illumination ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool FVisible(const CBaseEntity@ pEntity) const",
		asMETHODPR( CLASS, FVisible, ( const CBaseEntity* ) const, bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool FVisible(const Vector& in vecOrigin) const",
		asMETHODPR( CLASS, FVisible, ( const Vector& ) const, bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool FBoxVisible(const CBaseEntity@ pTarget, Vector& out vecTargetOrigin, float flSize = 0.0f) const",
		asMETHOD( CLASS, FBoxVisible ), asCALL_THISCALL );
}

inline void RegisterScriptCBaseEntity( asIScriptEngine& engine )
{
	RegisterScriptCBaseEntity<CBaseEntity>( engine, AS_CBASEENTITY_NAME );
}

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCBASEENTITY_H
