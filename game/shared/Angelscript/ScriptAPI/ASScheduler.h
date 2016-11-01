#ifndef GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ASSCHEDULER_H
#define GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ASSCHEDULER_H

class asIScriptEngine;

/**
*	Registers the interface to get the scheduler in script modules: the Scheduler global.
*	@param engine Script engine.
*/
void RegisterScriptSchedulerInterface( asIScriptEngine& engine );

#endif //GAME_SHARED_ANGELSCRIPT_SCRIPTAPI_ASSCHEDULER_H
