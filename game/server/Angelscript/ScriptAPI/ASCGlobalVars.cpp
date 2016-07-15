#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ASCGlobalVars.h"

static std::string CGlobalVars_mapname( const globalvars_t* pThis )
{
	return STRING( pThis->mapname );
}

static std::string CGlobalVars_startspot( const globalvars_t* pThis )
{
	return STRING( pThis->startspot );
}

static CBaseEntity* CGlobalVars_trace_ent( const globalvars_t* pThis )
{
	//Instance uses the world if it's given a null pointer, so avoid that.
	if( pThis->trace_ent )
		return CBaseEntity::Instance( pThis->trace_ent );

	return nullptr;
}

/**
*	Class name for CGlobalVars in scripts.
*/
#define AS_CGLOBALVARS_CLASSNAME "CGlobalVars"

void RegisterScriptCGlobalVars( asIScriptEngine& engine )
{
	const char* const pszObjectName = AS_CGLOBALVARS_CLASSNAME;

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectProperty(
		pszObjectName, "const float time", 
		asOFFSET( globalvars_t, time ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const float frametime",
		asOFFSET( globalvars_t, frametime ) );

	engine.RegisterObjectProperty(
		pszObjectName, "float force_retouch",
		asOFFSET( globalvars_t, force_retouch ) );

	//TODO: expose string_t
	engine.RegisterObjectMethod(
		pszObjectName, "string get_mapname() const",
		asFUNCTION( CGlobalVars_mapname ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "string startspot() const",
		asFUNCTION( CGlobalVars_startspot ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectProperty(
		pszObjectName, "const float deathmatch",
		asOFFSET( globalvars_t, deathmatch ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const float coop",
		asOFFSET( globalvars_t, coop ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const float teamplay",
		asOFFSET( globalvars_t, teamplay ) );

	engine.RegisterObjectProperty(
		pszObjectName, "float serverflags",
		asOFFSET( globalvars_t, serverflags ) );

	engine.RegisterObjectProperty(
		pszObjectName, "float found_secrets",
		asOFFSET( globalvars_t, found_secrets ) );

	//TODO: register Vector

	/*
	Vector		v_forward;
	Vector		v_up;
	Vector		v_right;
	*/

	engine.RegisterObjectProperty(
		pszObjectName, "const float trace_allsolid",
		asOFFSET( globalvars_t, trace_allsolid ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const float trace_startsolid",
		asOFFSET( globalvars_t, trace_startsolid ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const float trace_fraction",
		asOFFSET( globalvars_t, trace_fraction ) );

	/*
	Vector		trace_endpos;
	Vector		trace_plane_normal;
	*/

	engine.RegisterObjectProperty(
		pszObjectName, "const float trace_plane_dist",
		asOFFSET( globalvars_t, trace_plane_dist ) );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseEntity@ get_trace_ent() const",
		asFUNCTION( CGlobalVars_trace_ent ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectProperty(
		pszObjectName, "const float trace_inopen",
		asOFFSET( globalvars_t, trace_inopen ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const float trace_inwater",
		asOFFSET( globalvars_t, trace_inwater ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const int trace_hitgroup",
		asOFFSET( globalvars_t, trace_hitgroup ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const int trace_flags",
		asOFFSET( globalvars_t, trace_flags ) );

	//TODO: doesn't seem to be used at all.
	engine.RegisterObjectProperty(
		pszObjectName, "const int msg_entity",
		asOFFSET( globalvars_t, msg_entity ) );

	engine.RegisterObjectProperty(
		pszObjectName, "int cdAudioTrack",
		asOFFSET( globalvars_t, cdAudioTrack ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const int maxClients",
		asOFFSET( globalvars_t, maxClients ) );

	engine.RegisterObjectProperty(
		pszObjectName, "const int maxEntities",
		asOFFSET( globalvars_t, maxEntities ) );

	//TODO: allow access to save data? is it ever even set?
	//void		*pSaveData;
	//Vector		vecLandmarkOffset;

	engine.RegisterGlobalProperty( AS_CGLOBALVARS_CLASSNAME " Globals", gpGlobals );
}