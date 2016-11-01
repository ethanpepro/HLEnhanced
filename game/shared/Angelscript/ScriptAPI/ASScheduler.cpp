#include <angelscript.h>

#include "Angelscript/CASModule.h"
#include "Angelscript/ScriptAPI/CASScheduler.h"

#include "ASScheduler.h"

static CASScheduler* Script_get_Scheduler()
{
	auto pModule = GetModuleFromScriptContext( asGetActiveContext() );

	if( !pModule )
		return nullptr;

	return pModule->GetScheduler();
}

void RegisterScriptSchedulerInterface( asIScriptEngine& engine )
{
	engine.RegisterGlobalFunction(
		"CScheduler@ get_Scheduler()",
		asFUNCTION( Script_get_Scheduler ), asCALL_CDECL );
}
