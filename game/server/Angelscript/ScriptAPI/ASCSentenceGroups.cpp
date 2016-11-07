#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ASCSentenceGroups.h"

static int CSentenceGroups_PlayRandomGroup( CSentenceGroups* pThis,
											CBaseEntity* pEntity, const std::string& szGroupName, float flVolume, float flAttenuation, int iFlags, int iPitch )
{
	return g_Sentences.PlayRndSz( pEntity, szGroupName.c_str(), flVolume, flAttenuation, iFlags, iPitch );
}

static int CSentenceGroups_PlaySequentialGroup( CSentenceGroups* pThis,
												CBaseEntity* pEntity, const std::string& szGroupName, 
												float flVolume, float flAttenuation, int iFlags, int iPitch, int iPick, const bool bReset )
{
	return g_Sentences.PlaySequentialSz( pEntity, szGroupName.c_str(), flVolume, flAttenuation, iFlags, iPitch, iPick, bReset );
}

static int CSentenceGroups_GetIndex( CSentenceGroups* pThis,
									 const std::string& szGroupName )
{
	return g_Sentences.GetIndex( szGroupName.c_str() );
}

//TODO: CSentenceGroups::Lookup? - Solokiller

#define AS_CSENTENCEGROUPS_NAME "CSentenceGroups"

void RegisterScriptCSentenceGroups( asIScriptEngine& engine )
{
	engine.RegisterGlobalProperty( "const int INVALID_SENTENCE_INDEX", const_cast<int*>( &CSentenceGroups::INVALID_SENTENCE_INDEX ) );

	const char* const pszObjectName = AS_CSENTENCEGROUPS_NAME;

	engine.RegisterObjectType(
		pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "void Stop(CBaseEntity@ pEntity, int iSentenceGroup, int iPick)",
		asMETHOD( CSentenceGroups, Stop ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int PlayRandomIndex(CBaseEntity@ pEntity, int iSentenceGroup, float flVolume, float flAttenuation, int iFlags, int iPitch)",
		asMETHOD( CSentenceGroups, PlayRndI ), asCALL_THISCALL );

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
		AS_CSENTENCEGROUPS_NAME " Sentences", &g_Sentences );
}
