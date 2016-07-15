#include <Angelscript/util/ASExtendAdapter.h>

#include "extdll.h"
#include "util.h"

#include "shared_game_utils.h"

#include "CASMapModuleBuilder.h"

CASMapModuleBuilder::CASMapModuleBuilder( const char* const pszMapScript )
	: CASBaseModuleBuilder( "scripts/maps/" )
{
	AddInternalScript( "__ScriptGameRules", std::move( as::CreateExtendBaseclassDeclaration( "CScriptGameRules", "IGameRules", "CGameRules", "GameRules" ) ) );

	if( pszMapScript && *pszMapScript )
	{
		AddScript( pszMapScript );
	}

	Alert( at_console, "Beginning map script compilation...\n" );
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