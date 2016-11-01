#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASCSERVERENGINE_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASCSERVERENGINE_H

class asIScriptEngine;

/**
*	Registers enginefuncs_t as CServerEngine, provides the g_engfuncs global.
*	@param engine Script engine.
*/
void RegisterScriptCServerEngine( asIScriptEngine& engine );

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASCSERVERENGINE_H
