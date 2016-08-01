#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules/GameRules.h"

#include <Angelscript/CASModule.h>
#include <Angelscript/util/ASExtendAdapter.h>
#include <Angelscript/wrapper/ASCallable.h>

#include "Angelscript/HLASConstants.h"

#include "Angelscript/CHLASServerInitializer.h"

#include "Angelscript/CASMapModuleBuilder.h"

#include "ScriptAPI/Extensions/CASGameRules.h"

#include "CHLASServerManager.h"

CHLASServerManager g_ASManager;

bool CHLASServerManager::Initialize()
{
	CHLASServerInitializer initializer( *this );

	if( !m_Manager.Initialize( initializer ) )
	{
		ALERT( at_console, "Failed to initialize Angelscript\n" );
		return false;
	}

	//Map scripts are per-map scripts that always have their hooks executed before any other module.
	auto descriptor = m_Manager.GetModuleManager().AddDescriptor( "MapScript", ModuleAccessMask::MAPSCRIPT, as::ModulePriority::HIGHEST );

	if( !descriptor.first )
	{
		ALERT( at_console, "Failed to add MapScript module type\n" );
		return false;
	}

	return true;
}

void CHLASServerManager::Shutdown()
{
	if( m_pModule )
	{
		m_Manager.GetModuleManager().RemoveModule( m_pModule );
		m_pModule = nullptr;
	}

	CHLASManager::Shutdown();
}

CGameRules* CHLASServerManager::CreateGameRules()
{
	if( !m_pModule )
		return nullptr;

	//Because this is an extension class, and we don't have any way to create those properly yet, ask for the classname and create it.
	if( auto pFunction = m_pModule->GetModule()->GetFunctionByDecl( "bool InstallGameRules(string& out)" ) )
	{
		CASOwningContext ctx( *m_Manager.GetEngine() );

		CASFunction function( *pFunction, ctx );

		std::string szName;

		if( function.Call( CallFlag::NONE, &szName ) )
		{
			bool bResult = false;

			if( function.GetReturnValue( &bResult ) && bResult )
			{
				return as::CreateExtensionClassInstance<CASGameRules>( *m_Manager.GetEngine(), *m_pModule->GetModule(), szName.c_str(), "CGameRules", "GameRules" );
			}
		}
	}

	return nullptr;
}

void CHLASServerManager::WorldCreated( const char* const pszMapScriptFileName )
{
	CASMapModuleBuilder builder( pszMapScriptFileName );

	m_pModule = m_Manager.GetModuleManager().BuildModule( "MapScript", "MapModule", builder );

	if( m_pModule )
	{
		//Call MapInit if it exists.
		if( auto pFunction = m_pModule->GetModule()->GetFunctionByDecl( "void MapInit()" ) )
		{
			as::Call( pFunction );
		}
	}
	else
	{
		ALERT( at_console, "Failed to create map script\n" );
	}
}

void CHLASServerManager::WorldActivated()
{
	if( m_pModule )
	{
		//Call MapActivate if it exists.
		if( auto pFunction = m_pModule->GetModule()->GetFunctionByDecl( "void MapActivate()" ) )
		{
			as::Call( pFunction );
		}
	}
}

void CHLASServerManager::WorldEnded()
{
	if( m_pModule )
	{
		//Call MapShutdown if it exists.
		if( auto pFunction = m_pModule->GetModule()->GetFunctionByDecl( "void MapShutdown()" ) )
		{
			as::Call( pFunction );
		}

		m_Manager.GetModuleManager().RemoveModule( m_pModule );
		m_pModule = nullptr;
	}
}