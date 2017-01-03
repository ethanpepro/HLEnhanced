#include <cstdarg>
#include <string>

#include <angelscript.h>

#include "Angelscript/HLASConstants.h"
//TODO: avoid using server specific code here. - Solokiller
#include "Angelscript/CHLASServerManager.h"

#include "ASEvents.h"

HookCallResult CallGlobalEvent( CASEvent& event, CallFlags_t flags, ... )
{
	va_list list;

	va_start( list, flags );

	CASEventCaller caller;

	const auto result = caller.VCall( event, g_ASManager.GetASManager().GetEngine(), flags, list );

	va_end( list );

	return result;
}

CASEvent g_ClientPutInServerEvent( "ClientPutInServer", "CBasePlayer@", "Player", ModuleAccessMask::ALL, EventStopMode::CALL_ALL );

CASEvent g_PlayerPreThinkEvent( "PreThink", "CBasePlayer@, uint32& out", "Player", ModuleAccessMask::ALL, EventStopMode::CALL_ALL );

CASEvent g_PlayerUseEvent( "Use", "CBasePlayer@, uint32& out", "Player", ModuleAccessMask::ALL, EventStopMode::ON_HANDLED );

CASEvent g_PlayerSayEvent( "Say", "CSayArgs@", "Player", ModuleAccessMask::ALL, EventStopMode::CALL_ALL );

static void RegisterScriptPreThinkFlag( asIScriptEngine& engine )
{
	const char* const pszObjectName = "PreThinkFlag";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "NONE", PreThinkFlag::NONE );
	engine.RegisterEnumValue( pszObjectName, "SKIP_VEHICLES", PreThinkFlag::SKIP_VEHICLES );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptUseFlag( asIScriptEngine& engine )
{
	const char* const pszObjectName = "UseFlag";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "NONE", UseFlag::NONE );
	engine.RegisterEnumValue( pszObjectName, "SKIP_USE", UseFlag::SKIP_USE );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

void RegisterScriptEventData( asIScriptEngine& engine )
{
	RegisterScriptPreThinkFlag( engine );
	RegisterScriptUseFlag( engine );
}
