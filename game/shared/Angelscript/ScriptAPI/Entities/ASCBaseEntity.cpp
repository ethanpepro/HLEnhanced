#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "../ASCEntBitSet.h"

#include "ASCBaseEntity.h"

/**
*	Registers FixAngleMode.
*	@param engine Script engine.
*/
static void RegisterScriptFixAngleMode( asIScriptEngine& engine )
{
	const char* const pszObjectName = "FixAngleMode";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "FIXANGLE_NO", FIXANGLE_NO );
	engine.RegisterEnumValue( pszObjectName, "FIXANGLE_SET", FIXANGLE_SET );
	engine.RegisterEnumValue( pszObjectName, "FIXANGLE_ADD_AVEL", FIXANGLE_ADD_AVEL );
}

/**
*	Registers MoveType.
*	@param engine Script engine.
*/
static void RegisterScriptMoveType( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MoveType";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_NONE", MOVETYPE_NONE );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_WALK", MOVETYPE_WALK );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_STEP", MOVETYPE_STEP );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_FLY", MOVETYPE_FLY );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_TOSS", MOVETYPE_TOSS );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_PUSH", MOVETYPE_PUSH );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_NOCLIP", MOVETYPE_NOCLIP );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_FLYMISSILE", MOVETYPE_FLYMISSILE );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_BOUNCE", MOVETYPE_BOUNCE );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_BOUNCEMISSILE", MOVETYPE_BOUNCEMISSILE );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_FOLLOW", MOVETYPE_FOLLOW );
	engine.RegisterEnumValue( pszObjectName, "MOVETYPE_PUSHSTEP", MOVETYPE_PUSHSTEP );
}

/**
*	Registers Solid.
*	@param engine Script engine.
*/
static void RegisterScriptSolid( asIScriptEngine& engine )
{
	const char* const pszObjectName = "Solid";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "SOLID_NOT", SOLID_NOT );
	engine.RegisterEnumValue( pszObjectName, "SOLID_TRIGGER", SOLID_TRIGGER );
	engine.RegisterEnumValue( pszObjectName, "SOLID_BBOX", SOLID_BBOX );
	engine.RegisterEnumValue( pszObjectName, "SOLID_SLIDEBOX", SOLID_SLIDEBOX );
	engine.RegisterEnumValue( pszObjectName, "SOLID_BSP", SOLID_BSP );
}

/**
*	Registers RenderMode.
*	@param engine Script engine.
*/
static void RegisterScriptRenderMode( asIScriptEngine& engine )
{
	const char* const pszObjectName = "RenderMode";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "kRenderNormal", kRenderNormal );
	engine.RegisterEnumValue( pszObjectName, "kRenderTransColor", kRenderTransColor );
	engine.RegisterEnumValue( pszObjectName, "kRenderTransTexture", kRenderTransTexture );
	engine.RegisterEnumValue( pszObjectName, "kRenderGlow", kRenderGlow );
	engine.RegisterEnumValue( pszObjectName, "kRenderTransAlpha", kRenderTransAlpha );
	engine.RegisterEnumValue( pszObjectName, "kRenderTransAdd", kRenderTransAdd );
}

/**
*	Registers RenderFX.
*	@param engine Script engine.
*/
static void RegisterScriptRenderFX( asIScriptEngine& engine )
{
	const char* const pszObjectName = "RenderFX";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "kRenderFxNone", kRenderFxNone );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxPulseSlow", kRenderFxPulseSlow );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxPulseFast", kRenderFxPulseFast );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxPulseSlowWide", kRenderFxPulseSlowWide );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxPulseFastWide", kRenderFxPulseFastWide );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxFadeSlow", kRenderFxFadeSlow );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxFadeFast", kRenderFxFadeFast );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxSolidSlow", kRenderFxSolidSlow );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxSolidFast", kRenderFxSolidFast );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxStrobeSlow", kRenderFxStrobeSlow );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxStrobeFast", kRenderFxStrobeFast );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxStrobeFaster", kRenderFxStrobeFaster );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxFlickerSlow", kRenderFxFlickerSlow );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxFlickerFast", kRenderFxFlickerFast );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxNoDissipation", kRenderFxNoDissipation );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxDistort", kRenderFxDistort );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxHologram", kRenderFxHologram );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxDeadPlayer", kRenderFxDeadPlayer );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxExplode", kRenderFxExplode );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxGlowShell", kRenderFxGlowShell );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxClampMinScale", kRenderFxClampMinScale );
	engine.RegisterEnumValue( pszObjectName, "kRenderFxLightMultiplier", kRenderFxLightMultiplier );
}

/**
*	Registers TakeDamageMode.
*	@param engine Script engine.
*/
static void RegisterScriptTakeDamageMode( asIScriptEngine& engine )
{
	const char* const pszObjectName = "TakeDamageMode";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "DAMAGE_NO", DAMAGE_NO );
	engine.RegisterEnumValue( pszObjectName, "DAMAGE_YES", DAMAGE_YES );
	engine.RegisterEnumValue( pszObjectName, "DAMAGE_AIM", DAMAGE_AIM );
}

/**
*	Registers DeadFlag.
*	@param engine Script engine.
*/
static void RegisterScriptDeadFlag( asIScriptEngine& engine )
{
	const char* const pszObjectName = "DeadFlag";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "DEAD_NO", DEAD_NO );
	engine.RegisterEnumValue( pszObjectName, "DEAD_DYING", DEAD_DYING );
	engine.RegisterEnumValue( pszObjectName, "DEAD_DEAD", DEAD_DEAD );
	engine.RegisterEnumValue( pszObjectName, "DEAD_RESPAWNABLE", DEAD_RESPAWNABLE );
	engine.RegisterEnumValue( pszObjectName, "DEAD_DISCARDBODY", DEAD_DISCARDBODY );
}

/**
*	Registers WaterLevel.
*	@param engine Script engine.
*/
static void RegisterScriptWaterLevel( asIScriptEngine& engine )
{
	const char* const pszObjectName = "WaterLevel";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "WATERLEVEL_DRY", WATERLEVEL_DRY );
	engine.RegisterEnumValue( pszObjectName, "WATERLEVEL_FEET", WATERLEVEL_FEET );
	engine.RegisterEnumValue( pszObjectName, "WATERLEVEL_WAIST", WATERLEVEL_WAIST );
	engine.RegisterEnumValue( pszObjectName, "WATERLEVEL_HEAD", WATERLEVEL_HEAD );
}

/**
*	Registers Contents.
*	@param engine Script engine.
*/
static void RegisterScriptContents( asIScriptEngine& engine )
{
	const char* const pszObjectName = "Contents";

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "CONTENTS_EMPTY", CONTENTS_EMPTY );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_SOLID", CONTENTS_SOLID );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_WATER", CONTENTS_WATER );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_SLIME", CONTENTS_SLIME );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_LAVA", CONTENTS_LAVA );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_SKY", CONTENTS_SKY );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_ORIGIN", CONTENTS_ORIGIN );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_CLIP", CONTENTS_CLIP );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_CURRENT_0", CONTENTS_CURRENT_0 );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_CURRENT_90", CONTENTS_CURRENT_90 );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_CURRENT_180", CONTENTS_CURRENT_180 );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_CURRENT_270", CONTENTS_CURRENT_270 );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_CURRENT_UP", CONTENTS_CURRENT_UP );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_CURRENT_DOWN", CONTENTS_CURRENT_DOWN );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_TRANSLUCENT", CONTENTS_TRANSLUCENT );
	engine.RegisterEnumValue( pszObjectName, "CONTENTS_LADDER", CONTENTS_LADDER );
	engine.RegisterEnumValue( pszObjectName, "CONTENT_FLYFIELD", CONTENT_FLYFIELD );
	engine.RegisterEnumValue( pszObjectName, "CONTENT_GRAVITY_FLYFIELD", CONTENT_GRAVITY_FLYFIELD );
	engine.RegisterEnumValue( pszObjectName, "CONTENT_FOG", CONTENT_FOG );
}

/**
*	Class name for KeyValueData in scripts.
*/
#define AS_KEYVALUEDATA_NAME "KeyValueData"

static std::string KeyValueData_szKeyName( const KeyValueData* pThis )
{
	return pThis->szKeyName;
}

static std::string KeyValueData_szValue( const KeyValueData* pThis )
{
	return pThis->szValue;
}

static bool KeyValueData_get_bHandled( const KeyValueData* pThis )
{
	return pThis->fHandled != 0;
}

static void KeyValueData_set_bHandled( KeyValueData* pThis, const bool bHandled )
{
	pThis->fHandled = bHandled;
}

/**
*	Registers KeyValueData.
*	szClassName is not exposed, it can be retrieved through other means.
*	fHandled becomes bHandled, and is a boolean.
*	@param engine Script engine.
*/
static void RegisterScriptKeyValueData( asIScriptEngine& engine )
{
	const char* const pszObjectName = AS_KEYVALUEDATA_NAME;

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "string get_szKeyName() const",
		asFUNCTION( KeyValueData_szKeyName ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "string get_szValue() const",
		asFUNCTION( KeyValueData_szValue ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "bool get_bHandled() const",
		asFUNCTION( KeyValueData_get_bHandled ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void set_bHandled(const bool bHandled)",
		asFUNCTION( KeyValueData_set_bHandled ), asCALL_CDECL_OBJFIRST );
}

/**
*	Class name for FCapability in scripts.
*/
#define AS_FCAPABILITY_NAME "FCapability"

static void RegisterScriptFCapability( asIScriptEngine& engine )
{
	const char* const pszObjectName = AS_FCAPABILITY_NAME;

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "FCAP_CUSTOMSAVE", FCAP_CUSTOMSAVE );
	engine.RegisterEnumValue( pszObjectName, "FCAP_ACROSS_TRANSITION", FCAP_ACROSS_TRANSITION );
	engine.RegisterEnumValue( pszObjectName, "FCAP_MUST_SPAWN", FCAP_MUST_SPAWN );
	engine.RegisterEnumValue( pszObjectName, "FCAP_DONT_SAVE", FCAP_DONT_SAVE );
	engine.RegisterEnumValue( pszObjectName, "FCAP_IMPULSE_USE", FCAP_IMPULSE_USE );
	engine.RegisterEnumValue( pszObjectName, "FCAP_CONTINUOUS_USE", FCAP_CONTINUOUS_USE );
	engine.RegisterEnumValue( pszObjectName, "FCAP_ONOFF_USE", FCAP_ONOFF_USE );
	engine.RegisterEnumValue( pszObjectName, "FCAP_DIRECTIONAL_USE", FCAP_DIRECTIONAL_USE );
	engine.RegisterEnumValue( pszObjectName, "FCAP_MASTER", FCAP_MASTER );
	engine.RegisterEnumValue( pszObjectName, "FCAP_FORCE_TRANSITION", FCAP_FORCE_TRANSITION );
}

/**
*	Class name for USE_TYPE in scripts.
*/
#define AS_USE_TYPE_NAME "USE_TYPE"

static void RegisterScriptUSE_TYPE( asIScriptEngine& engine )
{
	const char* const pszObjectName = AS_USE_TYPE_NAME;

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "USE_OFF", USE_OFF );
	engine.RegisterEnumValue( pszObjectName, "USE_ON", USE_ON );
	engine.RegisterEnumValue( pszObjectName, "USE_SET", USE_SET );
	engine.RegisterEnumValue( pszObjectName, "USE_TOGGLE", USE_TOGGLE );
}

/**
*	Class name for Classification in scripts.
*/
#define AS_CLASSIFICATION_NAME "Classification"

static void RegisterScriptClassification( asIScriptEngine& engine )
{
	const char* const pszObjectName = AS_CLASSIFICATION_NAME;

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "CLASS_NONE", CLASS_NONE );
	engine.RegisterEnumValue( pszObjectName, "CLASS_MACHINE", CLASS_MACHINE );
	engine.RegisterEnumValue( pszObjectName, "CLASS_PLAYER", CLASS_PLAYER );
	engine.RegisterEnumValue( pszObjectName, "CLASS_HUMAN_PASSIVE", CLASS_HUMAN_PASSIVE );
	engine.RegisterEnumValue( pszObjectName, "CLASS_HUMAN_MILITARY", CLASS_HUMAN_MILITARY );
	engine.RegisterEnumValue( pszObjectName, "CLASS_ALIEN_MILITARY", CLASS_ALIEN_MILITARY );
	engine.RegisterEnumValue( pszObjectName, "CLASS_ALIEN_PASSIVE", CLASS_ALIEN_PASSIVE );
	engine.RegisterEnumValue( pszObjectName, "CLASS_ALIEN_MONSTER", CLASS_ALIEN_MONSTER );
	engine.RegisterEnumValue( pszObjectName, "CLASS_ALIEN_PREY", CLASS_ALIEN_PREY );
	engine.RegisterEnumValue( pszObjectName, "CLASS_ALIEN_PREDATOR", CLASS_ALIEN_PREDATOR );
	engine.RegisterEnumValue( pszObjectName, "CLASS_INSECT", CLASS_INSECT );
	engine.RegisterEnumValue( pszObjectName, "CLASS_PLAYER_ALLY", CLASS_PLAYER_ALLY );
	engine.RegisterEnumValue( pszObjectName, "CLASS_PLAYER_BIOWEAPON", CLASS_PLAYER_BIOWEAPON );
	engine.RegisterEnumValue( pszObjectName, "CLASS_ALIEN_BIOWEAPON", CLASS_ALIEN_BIOWEAPON );
	engine.RegisterEnumValue( pszObjectName, "CLASS_IGNORE", CLASS_IGNORE );
}

/**
*	Class name for BloodColor in scripts.
*/
#define AS_BLOODCOLOR_NAME "BloodColor"

static void RegisterScriptBloodColor( asIScriptEngine& engine )
{
	const char* const pszObjectName = AS_BLOODCOLOR_NAME;

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "DONT_BLEED", DONT_BLEED );
	engine.RegisterEnumValue( pszObjectName, "BLOOD_COLOR_RED", BLOOD_COLOR_RED );
	engine.RegisterEnumValue( pszObjectName, "BLOOD_COLOR_YELLOW", BLOOD_COLOR_YELLOW );
	engine.RegisterEnumValue( pszObjectName, "BLOOD_COLOR_GREEN", BLOOD_COLOR_GREEN );
}

void RegisterScriptEntityDependencies( asIScriptEngine& engine )
{
	RegisterScriptFixAngleMode( engine );
	RegisterScriptMoveType( engine );
	RegisterScriptSolid( engine );
	RegisterScriptRenderMode( engine );
	RegisterScriptRenderFX( engine );
	RegisterScriptTakeDamageMode( engine );
	RegisterScriptDeadFlag( engine );
	RegisterScriptWaterLevel( engine );
	RegisterScriptContents( engine );
	RegisterScriptKeyValueData( engine );
	RegisterScriptFCapability( engine );
	RegisterScriptUSE_TYPE( engine );
	RegisterScriptClassification( engine );
	RegisterScriptBloodColor( engine );
	RegisterScriptCEntBitSet( engine );
}

std::string CBaseEntity_GetClassname( const CBaseEntity* pThis )
{
	return pThis->GetClassname();
}

bool CBaseEntity_ClassnameIs( const CBaseEntity* pThis, const std::string& szClassname )
{
	return pThis->ClassnameIs( szClassname.c_str() );
}

std::string CBaseEntity_GetGlobalName( const CBaseEntity* pThis )
{
	return pThis->GetGlobalName();
}

void CBaseEntity_SetGlobalName( CBaseEntity* pThis, const std::string& szGlobalName )
{
	pThis->SetGlobalName( ALLOC_STRING( szGlobalName.c_str() ) );
}

std::string CBaseEntity_GetTargetname( const CBaseEntity* pThis )
{
	return pThis->GetTargetname();
}

void CBaseEntity_SetTargetname( CBaseEntity* pThis, const std::string& szTargetname )
{
	pThis->SetTargetname( ALLOC_STRING( szTargetname.c_str() ) );
}

std::string CBaseEntity_GetTarget( const CBaseEntity* pThis )
{
	return pThis->GetTarget();
}

void CBaseEntity_SetTarget( CBaseEntity* pThis, const std::string& szTarget )
{
	pThis->SetTarget( ALLOC_STRING( szTarget.c_str() ) );
}

std::string CBaseEntity_GetNetName( const CBaseEntity* pThis )
{
	return pThis->GetNetName();
}

void CBaseEntity_SetNetName( CBaseEntity* pThis, const std::string& szNetName )
{
	pThis->SetNetName( ALLOC_STRING( szNetName.c_str() ) );
}

std::string CBaseEntity_GetModelName( const CBaseEntity* pThis )
{
	return pThis->GetModelName();
}

void CBaseEntity_SetModel( CBaseEntity* pThis, const std::string& szModelName )
{
	pThis->SetModel( ALLOC_STRING( szModelName.c_str() ) );
}

std::string CBaseEntity_GetViewModelName( const CBaseEntity* pThis )
{
	return pThis->GetViewModelName();
}

void CBaseEntity_SetViewModelName( CBaseEntity* pThis, const std::string& szViewModelName )
{
	pThis->SetViewModelName( ALLOC_STRING( szViewModelName.c_str() ) );
}

std::string CBaseEntity_GetWeaponModelName( const CBaseEntity* pThis )
{
	return pThis->GetWeaponModelName();
}

void CBaseEntity_SetWeaponModelName( CBaseEntity* pThis, const std::string& szWeaponModelName )
{
	pThis->SetWeaponModelName( ALLOC_STRING( szWeaponModelName.c_str() ) );
}

std::string CBaseEntity_GetMessage( const CBaseEntity* pThis )
{
	return pThis->GetMessage();
}

void CBaseEntity_SetMessage( CBaseEntity* pThis, const std::string& szMessage )
{
	pThis->SetMessage( ALLOC_STRING( szMessage.c_str() ) );
}
