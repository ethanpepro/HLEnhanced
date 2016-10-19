#include <memory>

#include <Angelscript/CASModule.h>

#include <Angelscript/ScriptAPI/CASScheduler.h>

#include "extdll.h"
#include "util.h"

#include "CFile.h"

#include "keyvalues/Keyvalues.h"

#include "CHLASServerManager.h"

#include "CASPluginModuleBuilder.h"

#include "CASPluginManager.h"

CASPluginManager::CASPluginManager( CHLASServerManager& ASManager )
	: m_ASManager( ASManager )
{
}

void CASPluginManager::WorldCreated()
{
	//Adjust all timers.
	for( auto pPlugin : m_PluginList )
	{
		pPlugin->GetScheduler()->AdjustTime( m_flPrevThinkTime - 1 );
	}

	//Reset to starting time. TODO: define this constant. - Solokiller
	m_flPrevThinkTime = 1;

	//Load plugin file if needed.
	if( !m_bLoadedPluginFile )
	{
		m_bLoadedPluginFile = true;

		LoadPluginsFile( "default_plugins.txt" );
	}

	//Initialize all plugins for this map.
	for( auto pPlugin : m_PluginList )
	{
		if( auto pFunction = pPlugin->GetModule()->GetFunctionByDecl( "void MapInit()" ) )
		{
			as::Call( pFunction );
		}
	}
}

void CASPluginManager::Think()
{
	for( auto pPlugin : m_PluginList )
	{
		pPlugin->GetScheduler()->Think( gpGlobals->time );
	}

	m_flPrevThinkTime = gpGlobals->time;
}

void CASPluginManager::LoadPluginsFile( const char* const pszPluginFile )
{
	ASSERT( pszPluginFile );
	ASSERT( *pszPluginFile );

	CFile file( pszPluginFile, "rb", "GAMECONFIG" );

	//TODO: use a parser that does this efficiently. - Solokiller

	if( !file.IsOpen() )
	{
		Alert( at_console, "CASPluginManager::LoadPluginsFile: Couldn't open plugin file \"%s\"\n", pszPluginFile );
		return;
	}

	const auto size = file.Size();

	auto data = std::make_unique<char[]>( size );

	const auto read = file.Read( data.get(), size );

	if( read != size )
	{
		Alert( at_console, "CASPluginManager::LoadPluginsFile: Failed to read file \"%s\" contents\n", pszPluginFile );
		return;
	}

	file.Close();

	kv::CKeyvaluesLexer::Memory_t memory( data.get(), size, false );

	kv::Parser parser( memory );

	const auto result = parser.Parse();

	if( result != kv::Parser::ParseResult::SUCCESS )
	{
		Alert( at_console, "CASPluginManager::LoadPluginsFile: Failed to parse file \"%s\" contents\n", pszPluginFile );
		return;
	}

	auto pKeyvalues = parser.GetKeyvalues();

	auto pPlugins = pKeyvalues->FindFirstChild<kv::Block>( "plugins" );

	if( !pPlugins )
	{
		Alert( at_console, "CASPluginManager::LoadPluginsFile: File \"%s\": no plugin list found\n", pszPluginFile );
		return;
	}

	for( const auto pKV : pPlugins->GetChildren() )
	{
		if( pKV->GetKey() != "plugin" )
		{
			Alert( at_console, "CASPluginManager::LoadPluginsFile: File \"%s\": encountered non-plugin block \"%s\" in list, ignoring\n", pszPluginFile, pKV->GetKey().CStr() );
			continue;
		}

		if( pKV->GetType() != kv::NodeType::BLOCK )
		{
			Alert( at_console, "CASPluginManager::LoadPluginsFile: File \"%s\": encountered non-block plugin entry, ignoring\n", pszPluginFile );
			continue;
		}

		auto pPlugin = static_cast<kv::Block*>( pKV );

		auto szName = pPlugin->FindFirstKeyvalue( "name" );

		auto szScript = pPlugin->FindFirstKeyvalue( "script" );

		if( szName.IsEmpty() || szScript.IsEmpty() )
		{
			Alert( at_console, "CASPluginManager::LoadPluginsFile: File \"%s\": encountered plugin with one or more missing parameters, ignoring\n", pszPluginFile );
			continue;
		}

		LoadPlugin( szName.CStr(), szScript.CStr() );
	}
}

bool CASPluginManager::LoadPlugin( const char* const pszName, const char* const pszScript )
{
	Alert( at_console, "Loading plugin \"%s\"\n", pszName );

	CASPluginModuleBuilder builder( pszScript );

	auto pModule = m_ASManager.GetASManager().GetModuleManager().BuildModule( "HotReloadablePlugin", pszName, builder );

	if( pModule )
	{
		m_PluginList.emplace_back( pModule );

		if( auto pFunction = pModule->GetModule()->GetFunctionByDecl( "void PluginInit()" ) )
		{
			as::Call( pFunction );
		}
	}

	return pModule != nullptr;
}
