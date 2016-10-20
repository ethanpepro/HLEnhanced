#include <Angelscript/util/ASExtendAdapter.h>

#include "extdll.h"
#include "util.h"

#include <Angelscript/add_on/scriptbuilder.h>

#include "shared_game_utils.h"

#include "CASMapModuleBuilder.h"

//TODO: split into server and client builders - Solokiller

CASMapModuleBuilder::CASMapModuleBuilder( const char* const pszMapScript )
	: CASBaseModuleBuilder( "scripts/maps/", "Map" )
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
	if( !CASBaseModuleBuilder::DefineWords( builder ) )
		return false;

	return true;
}
