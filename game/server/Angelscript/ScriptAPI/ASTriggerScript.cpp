#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "Angelscript/Entities/CTriggerScript.h"

#include "ASTriggerScript.h"

static std::string ASThinkStateToString( const CTriggerScript::ThinkState state )
{
	return CTriggerScript::ThinkStateToString( state );
}

static CTriggerScript::ThinkState ASThinkStateFromString( const std::string& szString )
{
	return CTriggerScript::ThinkStateFromString( szString.c_str() );
}

/**
*	Registers CTriggerScript::ThinkState.
*	@param engine Script engine.
*/
static void RegisterScriptCTriggerScriptThinkState( asIScriptEngine& engine )
{
	const char* const pszObjectName = "ThinkState";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "THINKING", static_cast<int>( CTriggerScript::ThinkState::THINKING ) );
	engine.RegisterEnumValue( pszObjectName, "STARTING", static_cast<int>( CTriggerScript::ThinkState::STARTING ) );
	engine.RegisterEnumValue( pszObjectName, "STOPPING", static_cast<int>( CTriggerScript::ThinkState::STOPPING ) );

	engine.RegisterGlobalFunction( 
		"string ToString(const ThinkState state)", 
		asFUNCTION( ASThinkStateToString ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"ThinkState FromString(const string& in szString)",
		asFUNCTION( ASThinkStateFromString ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

void RegisterScriptTriggerScript( asIScriptEngine& engine )
{
	RegisterScriptCTriggerScriptThinkState( engine );
}
