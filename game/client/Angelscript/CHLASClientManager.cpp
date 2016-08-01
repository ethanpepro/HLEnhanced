#include "extdll.h"
#include "util.h"

#include <Angelscript/CASModule.h>
#include <Angelscript/util/ASExtendAdapter.h>
#include <Angelscript/wrapper/ASCallable.h>

#include "Angelscript/HLASConstants.h"

#include "Angelscript/CHLASClientInitializer.h"

#include "Angelscript/CASMapModuleBuilder.h"

#include "CHLASClientManager.h"

CHLASClientManager g_ASManager;

bool CHLASClientManager::Initialize()
{
	CHLASClientInitializer initializer( *this );

	if( !InitializeManager( initializer ) )
		return false;

	//Map scripts are per-map scripts that always have their hooks executed before any other module.
	auto descriptor = m_Manager.GetModuleManager().AddDescriptor( "MapScript", ModuleAccessMask::MAPSCRIPT, as::ModulePriority::HIGHEST );

	if( !descriptor.first )
	{
		ALERT( at_console, "Failed to add MapScript module type\n" );
		return false;
	}

	return true;
}

void CHLASClientManager::Shutdown()
{
	if( m_pModule )
	{
		//The client doesn't know if the map is going to change, so clean up on shutdown. - Solokiller
		WorldEnded();
	}

	CHLASManager::Shutdown();
}

void CHLASClientManager::WorldCreated( const char* const pszMapScriptFileName )
{
	if( m_pModule )
	{
		//The client doesn't know if the map is going to change, so clean up on new map start. - Solokiller
		WorldEnded();
	}

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

void CHLASClientManager::WorldActivated()
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

void CHLASClientManager::WorldEnded()
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