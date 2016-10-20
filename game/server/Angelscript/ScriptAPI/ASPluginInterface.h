#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASPLUGININTERFACE_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASPLUGININTERFACE_H

class asIScriptEngine;

/**
*	Registers the plugin interface to scripts.
*	@param engine Script engine.
*/
void RegisterScriptPluginInterface( asIScriptEngine& engine );

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ASPLUGININTERFACE_H
