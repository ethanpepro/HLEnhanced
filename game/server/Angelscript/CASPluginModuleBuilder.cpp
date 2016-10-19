#include <Angelscript/util/ASExtendAdapter.h>

#include "extdll.h"
#include "util.h"

#include <Angelscript/add_on/scriptbuilder.h>

#include "shared_game_utils.h"

#include "CASPluginModuleBuilder.h"

CASPluginModuleBuilder::CASPluginModuleBuilder( const char* const pszScript )
	: CASBaseModuleBuilder( "scripts/plugins/" )
{
	if( pszScript && *pszScript )
	{
		AddScript( pszScript );
	}

	Alert( at_console, "Beginning plugin script compilation...\n" );
}

bool CASPluginModuleBuilder::DefineWords( CScriptBuilder& builder )
{
#ifdef CLIENT_DLL
	builder.DefineWord( "CLIENT_DLL" );
#else
	builder.DefineWord( "SERVER_DLL" );
#endif

	return true;
}

bool CASPluginModuleBuilder::PreBuild( CScriptBuilder& builder )
{
	const auto& scripts = GetScripts();

	Alert( at_console, "%u script%s\nCompiling...\n", scripts.size(), scripts.size() == 1 ? "" : "s" );

	return true;
}

bool CASPluginModuleBuilder::PostBuild( CScriptBuilder& builder, const bool bSuccess, CASModule* pModule )
{
	Alert( at_console, "Done\nPlugin script compilation %s\n", bSuccess ? "succeeded" : "failed" );

	return true;
}