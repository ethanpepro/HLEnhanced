#include <string>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include <Angelscript/CASModule.h>

#include "Angelscript/CHLASServerManager.h"
#include "Angelscript/CASPluginData.h"

#include "CTriggerScript.h"

BEGIN_DATADESC( CTriggerScript )
	DEFINE_FIELD( m_iszFunctionName, FIELD_STRING ),
	DEFINE_FIELD( m_flThinkInterval, FIELD_FLOAT ),
	DEFINE_FIELD( m_Mode, FIELD_INTEGER ),
	DEFINE_USEFUNC( TriggerUse ),
	DEFINE_USEFUNC( ToggleUse ),
	DEFINE_THINKFUNC( CallScriptThink ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_script, CTriggerScript );

const char* CTriggerScript::ThinkStateToString( const ThinkState state )
{
	switch( state )
	{
	case ThinkState::THINKING:	return "THINKING";
	case ThinkState::STARTING:	return "STARTING";
	case ThinkState::STOPPING:	return "STOPPING";
	}

	ASSERT( !"Invalid think state value!" );

	return "UNKNOWN";
}

CTriggerScript::ThinkState CTriggerScript::ThinkStateFromString( const char* const pszString )
{
	if( pszString && stricmp( "THINKING", pszString ) != 0 )
	{
		if( stricmp( "STARTING", pszString ) == 0 )
			return ThinkState::STARTING;

		if( stricmp( "STOPPING", pszString ) == 0 )
			return ThinkState::STOPPING;
	}

	return ThinkState::THINKING;
}

bool CTriggerScript::Restore( CRestore& restore )
{
	if( !BaseClass::Restore( restore ) )
		return false;

	//Need to find all functions again. Don't run the Spawn logic since we have to stick around; there were functions last time.
	//Don't run Spawn because we need to use restored think time.
	FindFunctions();

	return true;
}

void CTriggerScript::KeyValue( KeyValueData* pkvd )
{
	if( FStrEq( "functionName", pkvd->szKeyName ) )
	{
		m_iszFunctionName = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( "thinkInterval", pkvd->szKeyName ) )
	{
		m_flThinkInterval = strtod( pkvd->szValue, nullptr );
		pkvd->fHandled = true;
	}
	else if( FStrEq( "scriptCallMode", pkvd->szKeyName ) )
	{
		m_Mode = static_cast<Mode>( strtol( pkvd->szValue, nullptr, 10 ) );

		m_Mode = clamp( m_Mode, Mode::FIRST, Mode::LAST );

		pkvd->fHandled = true;
	}
	else
		BaseClass::KeyValue( pkvd );
}

void CTriggerScript::Spawn()
{
	FindFunctions();

	if( !HasFunctionName() || m_FunctionList.empty() )
	{
		Alert( at_warning, "%s(%s): No function name or no functions found; removing self\n", GetClassname(), GetTargetname() );
		UTIL_Remove( this );
		return;
	}

	if( IsThinkTrigger() )
	{
		SetUse( &CTriggerScript::ToggleUse );
		SetThink( &CTriggerScript::CallScriptThink );

		if( ShouldStartOn() )
			SetNextThink( NextThinkTime() );
		else
			SetNextThink( 0 );
	}
	else
	{
		SetUse( &CTriggerScript::TriggerUse );
		SetThink( nullptr );
	}
}

void CTriggerScript::TriggerUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue )
{
	SUB_UseTargets( pActivator, USE_TOGGLE, 0 );

	for( auto pFunction : m_FunctionList )
	{
		as::Call( pFunction, pActivator, pCaller, useType, flValue );
	}
}

void CTriggerScript::ToggleUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue )
{
	const bool bOn = GetNextThink() != 0;

	if( ShouldToggle( useType, bOn ) )
	{
		SUB_UseTargets( pActivator, USE_TOGGLE, 0 );

		if( ShouldUpdateThinkStates() )
		{
			CallThink( bOn ? ThinkState::STOPPING : ThinkState::STARTING );
		}

		if( bOn )
			SetNextThink( 0 );
		else
			SetNextThink( NextThinkTime() );
	}
}

void CTriggerScript::CallScriptThink()
{
	CallThink( ThinkState::THINKING );

	SetNextThink( NextThinkTime() );
}

void CTriggerScript::FindFunctions()
{
	if( HasFunctionName() )
	{
		std::string szFunctionSignature;

		const std::string szNamespace = as::ExtractNamespaceFromName( STRING( m_iszFunctionName ) );
		const std::string szName = as::ExtractNameFromName( STRING( m_iszFunctionName ) );

		if( IsThinkTrigger() )
		{
			szFunctionSignature = std::string( "void " ) + szName;

			if( ShouldUpdateThinkStates() )
				szFunctionSignature += "(ThinkState::ThinkState)";
			else
				szFunctionSignature += "()";
		}
		else
		{
			szFunctionSignature = std::string( "void " ) + szName + "(CBaseEntity@, CBaseEntity@, USE_TYPE, float)";
		}

		auto& moduleManager = g_ASManager.GetASManager().GetModuleManager();

		const auto uiModuleCount = moduleManager.GetModuleCount();

		std::string szOldNS;

		for( size_t uiIndex = 0; uiIndex < uiModuleCount; ++uiIndex )
		{
			auto pModule = moduleManager.FindModuleByIndex( uiIndex );

			ASSERT( pModule );

			//TODO: find a better way to identify modules.
			if( auto pData = CASModule_GetPluginData( pModule ) )
			{
				if( !ShouldCheckPlugins() )
					continue;

				//Skip plugins that don't have at least map lifetime.
				if( pData->GetLifetime() < PluginLifetime::MAP )
					continue;
			}
			else
			{
				if( ShouldIgnoreMapScripts() )
					continue;
			}

			auto pScriptModule = pModule->GetModule();

			szOldNS = pScriptModule->GetDefaultNamespace();

			//User input needs to be sanitized, so check the result to prevent potential garbage results.
			if( pScriptModule->SetDefaultNamespace( szNamespace.c_str() ) == 0 )
			{
				if( auto pFunction = pScriptModule->GetFunctionByDecl( szFunctionSignature.c_str() ) )
				{
					m_FunctionList.emplace_back( pFunction );
				}

				pScriptModule->SetDefaultNamespace( szOldNS.c_str() );
			}
		}
	}
}

void CTriggerScript::CallThink( const ThinkState state )
{
	//Call ignores arguments that the function doesn't specify, so this is safe.
	for( auto pFunction : m_FunctionList )
	{
		as::Call( pFunction, state );
	}
}
