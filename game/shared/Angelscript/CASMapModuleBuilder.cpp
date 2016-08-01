#include <Angelscript/util/ASExtendAdapter.h>

#include "extdll.h"
#include "util.h"

#include <Angelscript/add_on/scriptbuilder.h>

#include "shared_game_utils.h"

#include "CASMapModuleBuilder.h"

//TODO: split into server and client builders - Solokiller

CASMapModuleBuilder::CASMapModuleBuilder( const char* const pszMapScript )
	: CASBaseModuleBuilder( "scripts/maps/" )
{
#ifdef SERVER_DLL
	AddInternalScript( "__ScriptGameRules", std::move( as::CreateExtendBaseclassDeclaration( "CScriptGameRules", "IGameRules", "CGameRules", "GameRules" ) ) );
#endif

	if( pszMapScript && *pszMapScript )
	{
		AddScript( pszMapScript );
	}

	Alert( at_console, "Beginning map script compilation...\n" );
}

bool CASMapModuleBuilder::DefineWords( CScriptBuilder& builder )
{
#ifdef CLIENT_DLL
	builder.DefineWord( "CLIENT_DLL" );
#else
	builder.DefineWord( "SERVER_DLL" );
#endif

	return true;
}

bool CASMapModuleBuilder::PreBuild( CScriptBuilder& builder )
{
	const auto& scripts = GetScripts();

	Alert( at_console, "%u script%s\nCompiling...\n", scripts.size(), scripts.size() == 1 ? "" : "s" );

	return true;
}

bool CASMapModuleBuilder::PostBuild( CScriptBuilder& builder, const bool bSuccess, CASModule* pModule )
{
	Alert( at_console, "Done\nMap script compilation %s\n", bSuccess ? "succeeded" : "failed" );

	return true;
}