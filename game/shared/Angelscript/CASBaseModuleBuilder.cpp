#include <algorithm>
#include <cassert>

#include <Angelscript/add_on/scriptbuilder.h>

#include "extdll.h"
#include "util.h"

#include "CASBaseModuleBuilder.h"

CASBaseModuleBuilder::CASBaseModuleBuilder( std::string&& szBasePath, std::string&& szModuleTypeName )
	: m_szBasePath( std::move( szBasePath ) )
	, m_szModuleTypeName( std::move( szModuleTypeName ) )
{
}

bool CASBaseModuleBuilder::HasInternalScript( const char* const pszName ) const
{
	assert( pszName );

	if( !pszName )
		return false;

	auto it = std::find_if( m_InternalScripts.begin(), m_InternalScripts.end(), 
				  [ = ]( const auto& script )
	{
		return script.first == pszName;
	} );

	return it != m_InternalScripts.end();
}

bool CASBaseModuleBuilder::AddInternalScript( std::string&& szName, std::string&& szContents )
{
	if( HasInternalScript( szName.c_str() ) )
	{
		return false;
	}

	m_InternalScripts.emplace_back( std::make_pair( std::move( szName ), std::move( szContents ) ) );

	return true;
}

bool CASBaseModuleBuilder::HasScript( const char* const pszName ) const
{
	assert( pszName );

	if( !pszName )
		return false;

	auto it = std::find_if( m_Scripts.begin(), m_Scripts.end(),
							[ = ]( const auto& script )
	{
		return script == pszName;
	} );

	return it != m_Scripts.end();
}

bool CASBaseModuleBuilder::AddScript( std::string&& szName )
{
	if( HasScript( szName.c_str() ) )
		return false;

	m_Scripts.emplace_back( std::move( szName ) );

	return true;
}

bool CASBaseModuleBuilder::DefineWords( CScriptBuilder& builder )
{
#ifdef CLIENT_DLL
	builder.DefineWord( "CLIENT_DLL" );
#else
	builder.DefineWord( "SERVER_DLL" );
#endif

	return true;
}

bool CASBaseModuleBuilder::AddScripts( CScriptBuilder& builder )
{
	for( auto& script : m_InternalScripts )
	{
		if( builder.AddSectionFromMemory( script.first.c_str(), script.second.c_str() ) < 0 )
		{
			Alert( at_console, "CASBaseModuleBuilder::AddScripts: Error adding internal script \"%s\"\n", script.first.c_str() );
			return false;
		}
	}

	char szRelativePath[ MAX_PATH ];

	for( auto& script : m_Scripts )
	{
		const int iResult = snprintf( szRelativePath, sizeof( szRelativePath ), "%s%s.as", m_szBasePath.c_str(), script.c_str() );

		if( iResult < 0 || static_cast<size_t>( iResult ) >= sizeof( szRelativePath ) )
		{
			Alert( at_console, "CASBaseModuleBuilder::AddScripts: Error formatting map script \"%s\" path!\n", script.c_str() );
			return false;
		}

		char szAbsolutePath[ MAX_PATH ];

		if( g_pFileSystem->GetLocalPath( szRelativePath, szAbsolutePath, sizeof( szAbsolutePath ) ) )
		{
			if( builder.AddSectionFromFile( szAbsolutePath ) < 0 )
			{
				Alert( at_console, "CASBaseModuleBuilder::AddScripts: Error adding script \"%s\"!\n", szRelativePath );
				return false;
			}
		}
		else
		{
			Alert( at_console, "CASBaseModuleBuilder::AddScripts: Couldn't find map script \"%s\"!\n", szRelativePath );
			return false;
		}
	}

	return true;
}

bool CASBaseModuleBuilder::PreBuild( CScriptBuilder& builder )
{
	const auto& scripts = GetScripts();

	Alert( at_console, "%u script%s\nCompiling...\n", scripts.size(), scripts.size() == 1 ? "" : "s" );

	return true;
}

bool CASBaseModuleBuilder::PostBuild( CScriptBuilder& builder, const bool bSuccess, CASModule* pModule )
{
	Alert( at_console, "Done\n%s script compilation %s\n", m_szModuleTypeName.c_str(), bSuccess ? "succeeded" : "failed" );

	return true;
}
