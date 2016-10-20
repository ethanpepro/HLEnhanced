#ifndef GAME_SHARED_ANGELSCRIPT_CASMAPMODULEBUILDER_H
#define GAME_SHARED_ANGELSCRIPT_CASMAPMODULEBUILDER_H

#include "CASBaseModuleBuilder.h"

/**
*	Builder for the map script.
*/
class CASMapModuleBuilder : public CASBaseModuleBuilder
{
public:
	/**
	*	Constructor.
	*	@param pszMapScript Map script to load.
	*/
	CASMapModuleBuilder( const char* const pszMapScript );

	bool DefineWords( CScriptBuilder& builder ) override;
};

#endif //GAME_SHARED_ANGELSCRIPT_CASMAPMODULEBUILDER_H