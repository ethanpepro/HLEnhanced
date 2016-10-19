#ifndef GAME_SERVER_ANGELSCRIPT_CASPLUGINMODULEBUILDER_H
#define GAME_SERVER_ANGELSCRIPT_CASPLUGINMODULEBUILDER_H

#include "Angelscript/CASBaseModuleBuilder.h"

//TODO: this class shares a lot of code with the map module builder. - Solokiller
class CASPluginModuleBuilder : public CASBaseModuleBuilder
{
public:
	/**
	*	Constructor.
	*	@param pszScript Script to load.
	*/
	CASPluginModuleBuilder( const char* const pszScript );

	bool DefineWords( CScriptBuilder& builder ) override;

	bool PreBuild( CScriptBuilder& builder ) override;

	bool PostBuild( CScriptBuilder& builder, const bool bSuccess, CASModule* pModule ) override;
};

#endif //GAME_SERVER_ANGELSCRIPT_CASPLUGINMODULEBUILDER_H
