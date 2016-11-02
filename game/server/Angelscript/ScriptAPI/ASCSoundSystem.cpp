#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ASCSoundSystem.h"

static void RegisterScriptSoundFlag( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SoundFlag";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "SPAWNING", SND_SPAWNING );
	engine.RegisterEnumValue( pszObjectName, "STOP", SND_STOP );
	engine.RegisterEnumValue( pszObjectName, "CHANGE_VOL", SND_CHANGE_VOL );
	engine.RegisterEnumValue( pszObjectName, "CHANGE_PITCH", SND_CHANGE_PITCH );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptAttenuation( asIScriptEngine& engine )
{
	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "Attn" );

	engine.RegisterGlobalProperty( "const float NONE", const_cast<float*>( &ATTN_NONE ) );
	engine.RegisterGlobalProperty( "const float NORM", const_cast<float*>( &ATTN_NORM ) );
	engine.RegisterGlobalProperty( "const float IDLE", const_cast<float*>( &ATTN_IDLE ) );
	engine.RegisterGlobalProperty( "const float STATIC", const_cast<float*>( &ATTN_STATIC ) );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptPitch( asIScriptEngine& engine )
{
	const char* const pszObjectName = "Pitch";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "NORM", PITCH_NORM );
	engine.RegisterEnumValue( pszObjectName, "LOW", PITCH_LOW );
	engine.RegisterEnumValue( pszObjectName, "HIGH", PITCH_HIGH );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptSoundChan( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SoundChan";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "AUTO", CHAN_AUTO );
	engine.RegisterEnumValue( pszObjectName, "WEAPON", CHAN_WEAPON );
	engine.RegisterEnumValue( pszObjectName, "VOICE", CHAN_VOICE );
	engine.RegisterEnumValue( pszObjectName, "ITEM", CHAN_ITEM );
	engine.RegisterEnumValue( pszObjectName, "BODY", CHAN_BODY );
	engine.RegisterEnumValue( pszObjectName, "STREAM", CHAN_STREAM );
	engine.RegisterEnumValue( pszObjectName, "STATIC", CHAN_STATIC );

	engine.RegisterEnumValue( pszObjectName, "BOT", CHAN_BOT );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static int CSoundSystem_PrecacheSound( enginefuncs_t* pThis, const std::string& szSoundName )
{
	return pThis->pfnPrecacheSound( STRING( ALLOC_STRING( szSoundName.c_str() ) ) );
}

static void CSoundSystem_EmitAmbientSound( enginefuncs_t* pThis, 
										   CBaseEntity* pEntity, const Vector& vecOrigin, 
										   const std::string& szSample, 
										   float flVolume, float flAttenuation, int iFlags, int iPitch )
{
	UTIL_EmitAmbientSound( pEntity, vecOrigin, szSample.c_str(), flVolume, flAttenuation, iFlags, iPitch );
}

static void CSoundSystem_EmitSoundDyn( enginefuncs_t* pThis, 
									   CBaseEntity* pEntity, SoundChannel channel, 
									   const std::string& szSample, 
									   float flVolume, float flAttenuation, int iFlags, int iPitch )
{
	EMIT_SOUND_DYN( pEntity, channel, szSample.c_str(), flVolume, flAttenuation, iFlags, iPitch );
}

static void CSoundSystem_EmitSound( enginefuncs_t* pThis,
									   CBaseEntity* pEntity, SoundChannel channel,
									   const std::string& szSample,
									   float flVolume, float flAttenuation )
{
	EMIT_SOUND( pEntity, channel, szSample.c_str(), flVolume, flAttenuation );
}

static void CSoundSystem_StopSound( enginefuncs_t* pThis,
									CBaseEntity* pEntity, SoundChannel channel,
									const std::string& szSample )
{
	STOP_SOUND( pEntity, channel, szSample.c_str() );
}

static void CSoundSystem_EmitSoundSuit( enginefuncs_t* pThis,
									CBaseEntity* pEntity, const std::string& szSample )
{
	EMIT_SOUND_SUIT( pEntity, szSample.c_str() );
}

static void CSoundSystem_EmitGroupIDSuit( enginefuncs_t* pThis,
										  CBaseEntity* pEntity, int iSentenceGroup )
{
	EMIT_GROUPID_SUIT( pEntity, iSentenceGroup );
}

static void CSoundSystem_EmitGroupNameSuit( enginefuncs_t* pThis,
										CBaseEntity* pEntity, const std::string& szGroupName )
{
	EMIT_GROUPNAME_SUIT( pEntity, szGroupName.c_str() );
}

static float CSoundSystem_TextureType_PlaySound( enginefuncs_t* pThis,
												 const TraceResult& tr, Vector vecSrc, Vector vecEnd, int iBulletType )
{
	return TEXTURETYPE_PlaySound( tr, vecSrc, vecEnd, iBulletType );
}

/*
//TODO: should be a CBasePlayer* - Solokiller
static void CSoundSystem_FadeClientVolume( enginefuncs_t* pThis,
										   const CBaseEntity* pClient, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds )
{
	if( !pClient )
		return;

	pThis->pfnFadeClientVolume( pClient->edict(), fadePercent, fadeOutSeconds, holdTime, fadeInSeconds );
}
*/

#define AS_CSOUNDSYSTEM_NAME "CSoundSystem"

void RegisterScriptCSoundSystem( asIScriptEngine& engine )
{
	RegisterScriptSoundFlag( engine );
	RegisterScriptAttenuation( engine );
	RegisterScriptPitch( engine );
	RegisterScriptSoundChan( engine );

	//Sound functionality is registered as an object in case the engine is ever upgraded to have a dedicated sound system interface.
	//This will make conversion easier.

	const char* const pszObjectName = AS_CSOUNDSYSTEM_NAME;

	engine.RegisterObjectType(
		pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "int PrecacheSound(const string& in szSoundName)",
		asFUNCTION( CSoundSystem_PrecacheSound ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void EmitAmbientSound(CBaseEntity@ pEntity, const Vector& in vecOrigin, const string& in szSample, float flVolume, float flAttenuation, int iFlags, int iPitch)",
		asFUNCTION( CSoundSystem_EmitAmbientSound ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void EmitSoundDyn(CBaseEntity@ pEntity, SoundChan::SoundChan channel, const string& in szSample, float flVolume, float flAttenuation, int iFlags, int iPitch)",
		asFUNCTION( CSoundSystem_EmitSoundDyn ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void EmitSound(CBaseEntity@ pEntity, SoundChan::SoundChan channel, const string& in szSample, float flVolume, float flAttenuation)",
		asFUNCTION( CSoundSystem_EmitSound ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void StopSound(CBaseEntity@ pEntity, SoundChan::SoundChan channel, const string& in szSample)",
		asFUNCTION( CSoundSystem_StopSound ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void EmitSoundSuit(CBaseEntity@ pEntity, const string& in szSample)",
		asFUNCTION( CSoundSystem_EmitSoundSuit ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void EmitGroupIDSuit(CBaseEntity@ pEntity, int iSentenceGroup)",
		asFUNCTION( CSoundSystem_EmitGroupIDSuit ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void EmitGroupNameSuit(CBaseEntity@ pEntity, const string& in szGroupName)",
		asFUNCTION( CSoundSystem_EmitGroupNameSuit ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "float TextureType_PlaySound(const TraceResult& in tr, Vector vecSrc, Vector vecEnd, Bullet::Bullet bulletType )",
		asFUNCTION( CSoundSystem_TextureType_PlaySound ), asCALL_CDECL_OBJFIRST );

	//Sound fading doesn't actually work so don't expose this. - Solokiller
	/*
	engine.RegisterObjectMethod(
		pszObjectName, "void FadeClientVolume(const CBaseEntity@ pClient, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)",
		asFUNCTION( CSoundSystem_FadeClientVolume ), asCALL_CDECL_OBJFIRST );
		*/

	engine.RegisterGlobalProperty(
		AS_CSOUNDSYSTEM_NAME " SoundSystem", &g_engfuncs );
}
