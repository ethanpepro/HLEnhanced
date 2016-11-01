#include "extdll.h"
#include "util.h"

#include "Angelscript/CHLASManager.h"

#include "Angelscript/ScriptAPI/ASPluginInterface.h"

#include "Angelscript/ScriptAPI/Entities/ASEntityFuncs.h"

#include "Angelscript/ScriptAPI/ASCServerEngine.h"
#include "Angelscript/ScriptAPI/ASCSoundSystem.h"
#include "Angelscript/ScriptAPI/ASCSentenceGroups.h"
#include "Angelscript/ScriptAPI/ASEffects.h"
#include "Angelscript/ScriptAPI/ASUtilityFuncs.h"

#include "Angelscript/ScriptAPI/ASTriggerScript.h"

#include "Angelscript/ScriptAPI/ASGameRules.h"

#include "Angelscript/ScriptAPI/Extensions/CASGameRules.h"

#include "Angelscript/ScriptAPI/ASCustomEntities.h"

#if USE_AS_SQL
#include "Angelscript/ScriptAPI/SQL/ASHLSQL.h"
#endif

#include "CHLASServerInitializer.h"

bool CHLASServerInitializer::RegisterCoreAPI( CASManager& manager )
{
	CHLASBaseInitializer::RegisterCoreAPI( manager );

	auto& engine = *manager.GetEngine();

	RegisterScriptPluginInterface( engine );

	RegisterScriptCServerEngine( engine );
	RegisterScriptCSoundSystem( engine );
	RegisterScriptCSentenceGroups( engine );
	RegisterScriptEffects( engine );
	RegisterScriptUtilityFuncs( engine );

	//CBaseEntity is registered by CHLASBaseInitializer
	RegisterScriptEntityFuncs( engine );

	RegisterScriptTriggerScript( engine );

	engine.RegisterInterface( "IGameRules" );

	RegisterScriptCGameRules( engine );
	RegisterScriptGameRules( engine );

	RegisterScriptCustomEntities( engine );

#if USE_AS_SQL
	RegisterScriptHLSQL( engine );
#endif

	return true;
}