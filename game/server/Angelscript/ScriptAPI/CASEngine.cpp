#include <cassert>

#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>

#include "extdll.h"
#include "util.h"

#include "CASEngine.h"

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

void CASEngine::Print( const std::string& szString )
{
	g_engfuncs.pfnServerPrint( szString.c_str() );
}

static void CASEngine_Print( asIScriptGeneric* pArguments )
{
	const std::string& szFormat = *reinterpret_cast<const std::string*>( pArguments->GetArgAddress( 0 ) );

	char szBuffer[ 4096 ];

	if( as::SPrintf( szBuffer, szFormat.c_str(), 1, *pArguments ) )
	{
		g_engfuncs.pfnServerPrint( szBuffer );
	}
}

int CASEngine::PrecacheModel( const std::string& szFileName )
{
	return PRECACHE_MODEL( STRING( ALLOC_STRING( szFileName.c_str() ) ) );
}

int CASEngine::PrecacheGeneric( const std::string& szFileName )
{
	return PRECACHE_GENERIC( STRING( ALLOC_STRING( szFileName.c_str() ) ) );
}

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
	//TODO: determine a good maximum for varargs functions.

	const size_t uiMaxVarArgs = 8;

	engine.RegisterObjectMethod( 
		pszObjectName, "void Alert(ALERT_TYPE aType, const string& in szString)", 
		asMETHOD( CASEngine, Alert ), asCALL_THISCALL );

	as::RegisterVarArgsMethod( 
		engine, pszObjectName, "void", "Alert", "ALERT_TYPE aType, const string& in szFormat", 1, uiMaxVarArgs, asFUNCTION( CASEngine_Alert ) );

	engine.RegisterObjectMethod(
		pszObjectName, "void Print(const string& in szString)",
		asMETHOD( CASEngine, Print ), asCALL_THISCALL );

	as::RegisterVarArgsMethod(
		engine, pszObjectName, "void", "Print", "const string& in szFormat", 1, uiMaxVarArgs, asFUNCTION( CASEngine_Print ) );

	engine.RegisterObjectMethod(
		pszObjectName, "int PrecacheModel(const string& in szFileName)",
		asMETHOD( CASEngine, PrecacheModel ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int PrecacheGeneric(const string& in szFileName)",
		asMETHOD( CASEngine, PrecacheGeneric ), asCALL_THISCALL );

	engine.RegisterGlobalProperty( "CEngine Engine", &g_ASEngine );
}