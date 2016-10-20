#ifndef GAME_SERVER_ANGELSCRIPT_CASPLUGINMODULEBUILDER_H
#define GAME_SERVER_ANGELSCRIPT_CASPLUGINMODULEBUILDER_H

#include "Angelscript/CASBaseModuleBuilder.h"

class CASPluginModuleBuilder : public CASBaseModuleBuilder
{
public:
	/**
	*	Constructor.
	*	@param pszScript Script to load.
	*/
	CASPluginModuleBuilder( const char* const pszScript );

	bool DefineWords( CScriptBuilder& builder ) override;
};

#endif //GAME_SERVER_ANGELSCRIPT_CASPLUGINMODULEBUILDER_H
