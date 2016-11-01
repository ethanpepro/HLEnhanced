#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "com_model.h"

#include "ASUtilityFuncs.h"

static void RegisterScriptMoveToOrigin( asIScriptEngine& engine )
{
	const char* const pszObjectName = "MoveToOrigin";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "NORMAL", MOVE_NORMAL );
	engine.RegisterEnumValue( pszObjectName, "STRAFE", MOVE_STRAFE );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptIgnoreMonsters( asIScriptEngine& engine )
{
	const char* const pszObjectName = "IgnoreMonsters";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "DONT_IGNORE_MONSTERS", dont_ignore_monsters  );
	engine.RegisterEnumValue( pszObjectName, "IGNORE_MONSTERS", ignore_monsters );
	engine.RegisterEnumValue( pszObjectName, "MISSILE", missile );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptIgnoreGlass( asIScriptEngine& engine )
{
	const char* const pszObjectName = "IgnoreGlass";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "DONT_IGNORE_GLASS", dont_ignore_glass );
	engine.RegisterEnumValue( pszObjectName, "IGNORE_GLASS", ignore_glass );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

//TODO: move to ASHLConst.cpp - Solokiller
static void RegisterScriptHull( asIScriptEngine& engine )
{
	const char* const pszObjectName = "Hull";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "POINT", Hull::POINT );
	engine.RegisterEnumValue( pszObjectName, "HUMAN", Hull::HUMAN );
	engine.RegisterEnumValue( pszObjectName, "LARGE", Hull::LARGE );
	engine.RegisterEnumValue( pszObjectName, "HEAD", Hull::HEAD );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

//TODO: move to ASHLConst.cpp - Solokiller
static void RegisterScriptContents( asIScriptEngine& engine )
{
	const char* const pszObjectName = "Contents";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "EMPTY", CONTENTS_EMPTY );
	engine.RegisterEnumValue( pszObjectName, "SOLID", CONTENTS_SOLID );
	engine.RegisterEnumValue( pszObjectName, "WATER", CONTENTS_WATER );
	engine.RegisterEnumValue( pszObjectName, "SLIME", CONTENTS_SLIME );
	engine.RegisterEnumValue( pszObjectName, "LAVA", CONTENTS_LAVA );
	engine.RegisterEnumValue( pszObjectName, "SKY", CONTENTS_SKY );

	engine.RegisterEnumValue( pszObjectName, "ORIGIN", CONTENTS_ORIGIN );

	engine.RegisterEnumValue( pszObjectName, "CLIP", CONTENTS_CLIP );
	engine.RegisterEnumValue( pszObjectName, "CURRENT_0", CONTENTS_CURRENT_0 );
	engine.RegisterEnumValue( pszObjectName, "CURRENT_90", CONTENTS_CURRENT_90 );
	engine.RegisterEnumValue( pszObjectName, "CURRENT_180", CONTENTS_CURRENT_180 );
	engine.RegisterEnumValue( pszObjectName, "CURRENT_270", CONTENTS_CURRENT_270 );
	engine.RegisterEnumValue( pszObjectName, "CURRENT_UP", CONTENTS_CURRENT_UP );
	engine.RegisterEnumValue( pszObjectName, "CURRENT_DOWN", CONTENTS_CURRENT_DOWN );

	engine.RegisterEnumValue( pszObjectName, "TRANSLUCENT", CONTENTS_TRANSLUCENT );
	engine.RegisterEnumValue( pszObjectName, "LADDER", CONTENTS_LADDER );

	engine.RegisterEnumValue( pszObjectName, "FLYFIELD", CONTENT_FLYFIELD );
	engine.RegisterEnumValue( pszObjectName, "GRAVITY_FLYFIELD", CONTENT_GRAVITY_FLYFIELD );
	engine.RegisterEnumValue( pszObjectName, "FOG", CONTENT_FOG );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

namespace Util
{
//TODO: eventually the UTIL_* versions will use CBaseEntity, so these will become obsolete. - Solokiller
static void TraceLine( const Vector& vecStart, const Vector& vecEnd, IGNORE_MONSTERS igmon, CBaseEntity* pentIgnore, TraceResult& tr )
{
	UTIL_TraceLine( vecStart, vecEnd, igmon, pentIgnore ? pentIgnore->edict() : nullptr, &tr );
}

static void TraceLine( const Vector& vecStart, const Vector& vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, CBaseEntity* pentIgnore, TraceResult& tr )
{
	UTIL_TraceLine( vecStart, vecEnd, igmon, ignoreGlass, pentIgnore ? pentIgnore->edict() : nullptr, &tr );
}

static void TraceToss( CBaseEntity* pEntity, CBaseEntity* pentToIgnore, TraceResult& tr )
{
	g_engfuncs.pfnTraceToss( pEntity ? pEntity->edict() : nullptr, pentToIgnore ? pentToIgnore->edict() : nullptr, &tr );
}

static void TraceHull( const Vector& vecStart, const Vector& vecEnd, IGNORE_MONSTERS igmon, const Hull::Hull hullNumber, CBaseEntity* pentIgnore, TraceResult& tr )
{
	UTIL_TraceHull( vecStart, vecEnd, igmon, hullNumber, pentIgnore ? pentIgnore->edict() : nullptr, &tr );
}

static void TraceModel( const Vector& vecStart, const Vector& vecEnd, const Hull::Hull hullNumber, CBaseEntity* pentModel, TraceResult& tr )
{
	UTIL_TraceModel( vecStart, vecEnd, hullNumber, pentModel->edict(), &tr );
}

static std::string TraceTexture( CBaseEntity* pEntity, const Vector& vecStart, const Vector& vecEnd, bool& bOutFoundSurface )
{
	const auto pTexture = UTIL_TraceTexture( pEntity, vecStart, vecEnd );

	bOutFoundSurface = pTexture != nullptr;

	return pTexture ? pTexture->name : "";
}

static int DecalIndex( const std::string& szName )
{
	return g_engfuncs.pfnDecalIndex( szName.c_str() );
}
}

void RegisterScriptUtilityFuncs( asIScriptEngine& engine )
{
	RegisterScriptMoveToOrigin( engine );
	RegisterScriptIgnoreMonsters( engine );
	RegisterScriptIgnoreGlass( engine );
	RegisterScriptHull( engine );
	RegisterScriptContents( engine );

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "Util" );

	engine.RegisterGlobalFunction(
		"void MoveToOrigin(CBaseEntity@ pEntity, const Vector& in vecGoal, float flDist, MoveToOrigin::MoveToOrigin moveType)",
		asFUNCTION( UTIL_MoveToOrigin ), asCALL_CDECL );

	//TODO: change yaw and change pitch - Solokiller

	//TODO: move this to player funcs - Solokiller
	engine.RegisterGlobalFunction(
		"CBaseEntity@ FindClientInPVS(CBaseEntity@ pPVSEntity)",
		asFUNCTION( UTIL_FindClientInPVS ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void TraceLine(const Vector& in vecStart, const Vector& in vecEnd,"
		"IgnoreMonsters::IgnoreMonsters igmon, CBaseEntity@ pentIgnore, TraceResult& out tr)",
		asFUNCTIONPR( Util::TraceLine, ( const Vector&, const Vector&, IGNORE_MONSTERS, CBaseEntity*, TraceResult& ), void ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void TraceLine(const Vector& in vecStart, const Vector& in vecEnd,"
		"IgnoreMonsters::IgnoreMonsters igmon, IgnoreGlass::IgnoreGlass ignoreGlass, CBaseEntity@ pentIgnore, TraceResult& out tr)",
		asFUNCTIONPR( Util::TraceLine, ( const Vector&, const Vector&, IGNORE_MONSTERS, IGNORE_GLASS, CBaseEntity*, TraceResult& ), void ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void TraceToss(CBaseEntity@ pEntity, CBaseEntity@ pentToIgnore, TraceResult& out tr)",
		asFUNCTION( Util::TraceToss ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void TraceMonsterHull(CBaseEntity@ pEntity, const Vector& in v1, const Vector& in v2, IgnoreMonsters::IgnoreMonsters igmon, CBaseEntity@ pentToSkip, TraceResult& out tr)",
		asFUNCTION( UTIL_TraceMonsterHull ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void TraceHull(const Vector& in vecStart, const Vector& in vecEnd, IgnoreMonsters::IgnoreMonsters igmon, const Hull::Hull hullNumber, CBaseEntity@ pentToSkip, TraceResult& out tr)",
		asFUNCTION( Util::TraceHull ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void TraceModel(const Vector& in vecStart, const Vector& in vecEnd, const Hull::Hull hullNumber, CBaseEntity@ pentModel, TraceResult& out tr)",
		asFUNCTION( Util::TraceModel ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"string TraceTexture(CBaseEntity@ pEntity, const Vector& in vecStart, const Vector& in vecEnd, bool& out bOutFoundSurface)",
		asFUNCTION( Util::TraceTexture ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"TraceResult GetGlobalTrace()",
		asFUNCTION( UTIL_GetGlobalTrace ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"Vector GetAimVector(const CBaseEntity@ pEntity, float flSpeed)",
		asFUNCTION( UTIL_GetAimVector ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"int DecalIndex(const string& in szName)",
		asFUNCTION( Util::DecalIndex ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"Contents::Contents PointContents(const Vector& in vecPosition)",
		asFUNCTION( UTIL_PointContents ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}
