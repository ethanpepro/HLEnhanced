#include <cassert>

#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>

#include "extdll.h"
#include "util.h"

#include "Angelscript/HLASConstants.h"

#include "CASEngine.h"

static void RegisterScriptALERT_TYPE( asIScriptEngine& engine )
{
	const char* const pszObjectName = "ALERT_TYPE";

	auto result = engine.RegisterEnum( pszObjectName );

	assert( result >= 0 );

	result = engine.RegisterEnumValue( pszObjectName, "at_notice", at_notice );
	assert( result >= 0 );

	result = engine.RegisterEnumValue( pszObjectName, "at_console", at_console );
	assert( result >= 0 );

	result = engine.RegisterEnumValue( pszObjectName, "at_aiconsole", at_aiconsole );
	assert( result >= 0 );

	result = engine.RegisterEnumValue( pszObjectName, "at_warning", at_warning );
	assert( result >= 0 );

	result = engine.RegisterEnumValue( pszObjectName, "at_error", at_error );
	assert( result >= 0 );

	result = engine.RegisterEnumValue( pszObjectName, "at_logged", at_logged );
	assert( result >= 0 );
}

void CASEngine::Alert( ALERT_TYPE aType, const std::string& szString )
{
	ALERT( aType, "%s", szString.c_str() );
}

static void CASEngine_Alert( asIScriptGeneric* pArguments )
{
	const ALERT_TYPE aType = static_cast<ALERT_TYPE>( pArguments->GetArgDWord( 0 ) );
	const std::string& szFormat = *reinterpret_cast<const std::string*>( pArguments->GetArgAddress( 1 ) );

	char szBuffer[ 4096 ];

	if( as::SPrintf( szBuffer, szFormat.c_str(), 2, *pArguments ) )
	{
		ALERT( aType, "%s", szBuffer );
	}
}

//TODO: the client will need to set pfnTime to GetAbsoluteTime for this to work. - Solokiller
static float CASEngine_Time( CASEngine* pThis )
{
	return g_engfuncs.pfnTime();
}

/**
*	Class name for CEngine in scripts.
*/
#define AS_CENGINE_CLASSNAME "CEngine"

static CASEngine g_ASEngine;

void RegisterScriptCEngine( asIScriptEngine& engine )
{
	RegisterScriptALERT_TYPE( engine );

	const char* const pszObjectName = AS_CENGINE_CLASSNAME;

	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	//0 vararg variants are much easier to use, so define those separately.

	engine.RegisterObjectMethod( 
		pszObjectName, "void Alert(ALERT_TYPE aType, const string& in szString)", 
		asMETHOD( CASEngine, Alert ), asCALL_THISCALL );

	as::RegisterVarArgsMethod( 
		engine, pszObjectName, "void", "Alert", "ALERT_TYPE aType, const string& in szFormat", 1, AS_MAX_VARARGS, asFUNCTION( CASEngine_Alert ) );

	engine.RegisterObjectMethod(
		pszObjectName, "float Time()",
		asFUNCTION( CASEngine_Time ), asCALL_CDECL_OBJFIRST );

	engine.RegisterGlobalProperty( "CEngine Engine", &g_ASEngine );
}