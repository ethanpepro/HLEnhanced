#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules/GameRules.h"

#include <Angelscript/CASModule.h>
#include <Angelscript/util/ASExtendAdapter.h>
#include <Angelscript/wrapper/ASCallable.h>

#if USE_AS_SQL
#include <Angelscript/ScriptAPI/SQL/CASSQLThreadPool.h>
#endif

#include <Angelscript/ScriptAPI/CASScheduler.h>

#include "Angelscript/HLASConstants.h"

#include "Angelscript/CHLASServerInitializer.h"

#include "Angelscript/CASMapModuleBuilder.h"

#include "ScriptAPI/Extensions/CASGameRules.h"

#include "ScriptAPI/SQL/ASHLSQL.h"

#include "CHLASServerManager.h"

CHLASServerManager g_ASManager;

CHLASServerManager::CHLASServerManager()
	: m_PluginManager( *this )
{
}

bool CHLASServerManager::Initialize()
{
	CHLASServerInitializer initializer( *this );

	if( !InitializeManager( initializer ) )
		return false;

	if( !m_PluginManager.Initialize() )
		return false;

	//Map scripts are per-map scripts that always have their hooks executed before any other module.
	auto descriptor = m_Manager.GetModuleManager().AddDescriptor( "MapScript", ModuleAccessMask::MAPSCRIPT, as::ModulePriority::HIGHEST );

	if( !descriptor.first )
	{
		ALERT( at_console, "Failed to add MapScript module type\n" );
		return false;
	}

	descriptor = m_Manager.GetModuleManager().AddDescriptor( "Plugin", ModuleAccessMask::PLUGIN, as::ModulePriority::HIGHEST - 1 );

	if( !descriptor.first )
	{
		ALERT( at_console, "Failed to add HotReloadablePlugin module type\n" );
		return false;
	}

	return true;
}

void CHLASServerManager::Shutdown()
{
#if USE_AS_SQL
	g_pSQLThreadPool->Stop( false );
#endif

	if( m_pModule )
	{
		m_Manager.GetModuleManager().RemoveModule( m_pModule );
		m_pModule = nullptr;
	}

	m_PluginManager.Shutdown();

	CHLASManager::Shutdown();
}

void CHLASServerManager::Think()
{
#if USE_AS_SQL
	CASOwningContext ctx( *m_Manager.GetEngine() );
	g_pSQLThreadPool->ProcessQueue( *ctx.GetContext() );
#endif

	if( m_pModule )
	{
		m_pModule->GetScheduler()->Think( gpGlobals->time );
	}

	m_PluginManager.Think();
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

	if( !m_pModule )
	{
		ALERT( at_console, "Failed to create map script\n" );
	}

	m_PluginManager.WorldCreated();

	//Must be done after the plugin manager has done its thing so plugins are loaded.
	for( size_t uiIndex = 0; uiIndex < m_Manager.GetModuleManager().GetModuleCount(); ++uiIndex )
	{
		auto pModule = m_Manager.GetModuleManager().FindModuleByIndex( uiIndex );

		ASSERT( pModule );

		//Call MapInit if it exists.
		if( auto pFunction = pModule->GetModule()->GetFunctionByDecl( "void MapInit()" ) )
		{
			as::Call( pFunction );
		}
	}
}

void CHLASServerManager::WorldActivated()
{
	for( size_t uiIndex = 0; uiIndex < m_Manager.GetModuleManager().GetModuleCount(); ++uiIndex )
	{
		auto pModule = m_Manager.GetModuleManager().FindModuleByIndex( uiIndex );

		ASSERT( pModule );

		//Call MapActivate if it exists.
		if( auto pFunction = pModule->GetModule()->GetFunctionByDecl( "void MapActivate()" ) )
		{
			as::Call( pFunction );
		}
	}
}

void CHLASServerManager::WorldEnded()
{
	for( size_t uiIndex = 0; uiIndex < m_Manager.GetModuleManager().GetModuleCount(); ++uiIndex )
	{
		auto pModule = m_Manager.GetModuleManager().FindModuleByIndex( uiIndex );

		ASSERT( pModule );

		//Call MapShutdown if it exists.
		if( auto pFunction = pModule->GetModule()->GetFunctionByDecl( "void MapShutdown()" ) )
		{
			as::Call( pFunction );
		}
	}

	if( m_pModule )
	{
		m_Manager.GetModuleManager().RemoveModule( m_pModule );
		m_pModule = nullptr;
	}
}
