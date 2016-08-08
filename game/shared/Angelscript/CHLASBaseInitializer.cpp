#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include <Angelscript/add_on/scriptstdstring.h>
#include <Angelscript/add_on/scriptarray.h>
#include <Angelscript/add_on/scriptdictionary.h>
#include <Angelscript/add_on/scriptany.h>
#include <Angelscript/add_on/scriptbuilder.h>

#include <Angelscript/ScriptAPI/CASScheduler.h>
#include <Angelscript/ScriptAPI/Reflection/ASReflection.h>

#include "Angelscript/ScriptAPI/Entities/ASCBaseEntity.h"
#include "Angelscript/ScriptAPI/CASEngine.h"
#include "Angelscript/ScriptAPI/ASCGlobalVars.h"

#include "HLASConstants.h"

#include "CHLASManager.h"

#include "CHLASBaseInitializer.h"

bool CHLASBaseInitializer::GetMessageCallback( asSFuncPtr& outFuncPtr, void*& pOutObj, asDWORD& outCallConv )
{
	outFuncPtr = asMETHOD( CHLASManager, MessageCallback );
	pOutObj = &m_Manager;
	outCallConv = asCALL_THISCALL;

	return true;
}

bool CHLASBaseInitializer::RegisterCoreAPI( CASManager& manager )
{
	auto& engine = *manager.GetEngine();

	engine.SetDefaultAccessMask( ModuleAccessMask::ALL );

	Alert( at_console, "Angelscript v%s\nRegistering Angelscript API\n", ANGELSCRIPT_VERSION_STRING );

	RegisterStdString( &engine );
	RegisterScriptArray( &engine, true );
	RegisterScriptDictionary( &engine );
	RegisterScriptAny( &engine );
	RegisterScriptScheduler( &engine );
	RegisterScriptReflection( engine );

	RegisterScriptEntityDependencies( engine );
	RegisterScriptCBaseEntity( engine );
	RegisterScriptCEngine( engine );
	RegisterScriptCGlobalVars( engine );

	return true;
}

bool CHLASBaseInitializer::RegisterAPI( CASManager& manager )
{
	auto& engine = *manager.GetEngine();

	Alert( at_console, "Finished registering API\n%u Object types\n%u Global functions\n%u Global properties\n%u Enums\n%u Funcdefs\n%u Typedefs\n",
			engine.GetObjectTypeCount(),
			engine.GetGlobalFunctionCount(), engine.GetGlobalPropertyCount(),
			engine.GetEnumCount(), engine.GetFuncdefCount(), engine.GetTypedefCount() );

	return true;
}