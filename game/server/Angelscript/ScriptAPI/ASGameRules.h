#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASGAMERULES_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASGAMERULES_H

class asIScriptEngine;

/**
*	Registers the CGameRules class.
*	@param engine Script engine.
*/
void RegisterScriptCGameRules( asIScriptEngine& engine );

template<typename BASECLASS>
void GameRules_Think( BASECLASS* pThis )
{
	pThis->BASECLASS::Think();
}

/**
*	Registers the base versions of CGameRules methods. This allow scripts to call base class implementations.
*	@param engine Script engine.
*	@param pszObjectName Class name.
*	@tparam BASECLASS Class type.
*/
template<typename BASECLASS>
void RegisterScriptGameRules( asIScriptEngine& engine, const char* const pszObjectName )
{
	engine.RegisterObjectType( pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );
}

/**
*	Registers the base version of CGameRules.
*	@param engine Script engine.
*/
void RegisterScriptGameRules( asIScriptEngine& engine );

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASGAMERULES_H