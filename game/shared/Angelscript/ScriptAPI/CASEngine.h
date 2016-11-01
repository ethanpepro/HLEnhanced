#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CASENGINE_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CASENGINE_H

#include <string>

class asIScriptEngine;

class CASEngine final
{
public:
	CASEngine() = default;
	~CASEngine() = default;

	void Alert( ALERT_TYPE aType, const std::string& szString );

private:
	CASEngine( const CASEngine& ) = delete;
	CASEngine& operator=( const CASEngine& ) = delete;
};

/**
*	Registers the CEngine class.
*	@param engine Script engine.
*/
void RegisterScriptCEngine( asIScriptEngine& engine );

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CASENGINE_H