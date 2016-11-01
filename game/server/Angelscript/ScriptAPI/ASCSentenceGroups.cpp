#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ASCSentenceGroups.h"

static void CSentenceGroups_Stop( enginefuncs_t* pThis, 
								  CBaseEntity* pEntity, int iSentenceGroup, int iPick )
{
	SENTENCEG_Stop( pEntity, iSentenceGroup, iPick );
}

static int CSentenceGroups_PlayRandomIndex( enginefuncs_t* pThis,
											CBaseEntity* pEntity, int iSentenceGroup, float flVolume, float flAttenuation, int iFlags, int iPitch )
{
	return SENTENCEG_PlayRndI( pEntity, iSentenceGroup, flVolume, flAttenuation, iFlags, iPitch );
}

static int CSentenceGroups_PlayRandomGroup( enginefuncs_t* pThis,
											CBaseEntity* pEntity, const std::string& szGroupName, float flVolume, float flAttenuation, int iFlags, int iPitch )
{
	return SENTENCEG_PlayRndSz( pEntity, szGroupName.c_str(), flVolume, flAttenuation, iFlags, iPitch );
}

static int CSentenceGroups_PlaySequentialGroup( enginefuncs_t* pThis, 
												CBaseEntity* pEntity, const std::string& szGroupName, 
												float flVolume, float flAttenuation, int iFlags, int iPitch, int iPick, const bool bReset )
{
	return SENTENCEG_PlaySequentialSz( pEntity, szGroupName.c_str(), flVolume, flAttenuation, iFlags, iPitch, iPick, bReset );
}

static int CSentenceGroups_GetIndex( enginefuncs_t* pThis,
									 const std::string& szGroupName )
{
	return SENTENCEG_GetIndex( szGroupName.c_str() );
}

//TODO: SENTENCEG_Lookup? - Solokiller

#define AS_CSENTENCEGROUPS_NAME "CSentenceGroups"

void RegisterScriptCSentenceGroups( asIScriptEngine& engine )
{
	engine.RegisterGlobalProperty( "const int SENTENCEGROUPS_INVALID_INDEX", const_cast<int*>( &SENTENCEG_INVALID_INDEX ) );

	const char* const pszObjectName = AS_CSENTENCEGROUPS_NAME;

	engine.RegisterObjectType(
		pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "void Stop(CBaseEntity@ pEntity, int iSentenceGroup, int iPick)",
		asFUNCTION( CSentenceGroups_Stop ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int PlayRandomIndex(CBaseEntity@ pEntity, int iSentenceGroup, float flVolume, float flAttenuation, int iFlags, int iPitch)",
		asFUNCTION( CSentenceGroups_PlayRandomIndex ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int PlayRandomGroup(CBaseEntity@ pEntity, const string& in szGroupName, float flVolume, float flAttenuation, int iFlags, int iPitch)",
		asFUNCTION( CSentenceGroups_PlayRandomGroup ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int PlaySequentialGroup(CBaseEntity@ pEntity, const string& in szGroupName,"
		"float flVolume, float flAttenuation, int iFlags, int iPitch, int iPick, const bool bReset)",
		asFUNCTION( CSentenceGroups_PlaySequentialGroup ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetIndex(const string& in szGroupName) const",
		asFUNCTION( CSentenceGroups_GetIndex ), asCALL_CDECL_OBJFIRST );

	engine.RegisterGlobalProperty( 
		AS_CSENTENCEGROUPS_NAME " SentenceGroups", &g_engfuncs );
}
