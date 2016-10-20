#include <Angelscript/util/ASExtendAdapter.h>

#include "extdll.h"
#include "util.h"

#include <Angelscript/add_on/scriptbuilder.h>

#include "shared_game_utils.h"

#include "CASPluginModuleBuilder.h"

CASPluginModuleBuilder::CASPluginModuleBuilder( const char* const pszScript )
	: CASBaseModuleBuilder( "scripts/plugins/", "Plugin" )
{
	if( pszScript && *pszScript )
	{
		AddScript( pszScript );
	}

	Alert( at_console, "Beginning plugin script compilation...\n" );
}

bool CASPluginModuleBuilder::DefineWords( CScriptBuilder& builder )
{
	if( !CASBaseModuleBuilder::DefineWords( builder ) )
		return false;

	return true;
}
