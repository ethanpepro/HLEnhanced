#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ASEHANDLE.h"

static void DefaultConstructEHANDLE( void* pMemory )
{
	new( pMemory ) EHANDLE();
}

static void EntityConstructEHANDLE( void* pMemory, CBaseEntity* pEntity )
{
	new( pMemory ) EHANDLE( pEntity );
}

static void DestructEHANDLE( EHANDLE* pEHandle )
{
	pEHandle->~EHANDLE();
}

static bool EHANDLE_operatorbool( const EHANDLE* pThis )
{
	return pThis->GetEntity() != nullptr;
}

void RegisterScriptEHANDLE( asIScriptEngine& engine )
{
	const char* const pszObjectName = "EHANDLE";

	engine.RegisterObjectType(
		pszObjectName, sizeof( EHANDLE ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void EHANDLE()",
		asFUNCTION( DefaultConstructEHANDLE ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void EHANDLE(CBaseEntity@ pEntity)",
		asFUNCTION( EntityConstructEHANDLE ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_DESTRUCT, "void EHANDLE()",
		asFUNCTION( DestructEHANDLE ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "EHANDLE& opAssign(const EHANDLE& in other)",
		asMETHODPR( EHANDLE, operator=, ( const EHANDLE& ), EHANDLE& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseEntity@ opAssign(CBaseEntity@ pEntity)",
		asMETHODPR( EHANDLE, operator=, ( CBaseEntity* ), CBaseEntity* ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool opImplConv() const",
		asFUNCTION( EHANDLE_operatorbool ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "CBaseEntity@ opImplConv()",
		asMETHOD( EHANDLE, operator CBaseEntity* ), asCALL_THISCALL );
}
