#include <string>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "gamerules/GameRules.h"

//TODO: Should move away from platform specific macros altogether
#undef min
#undef max
#undef VOID

#include <Angelscript/IASInitializer.h>
#include <Angelscript/IASModuleBuilder.h>
#include <Angelscript/CASModule.h>

#include <Angelscript/add_on/scriptstdstring.h>
#include <Angelscript/add_on/scriptarray.h>
#include <Angelscript/add_on/scriptdictionary.h>
#include <Angelscript/add_on/scriptany.h>
#include <Angelscript/add_on/scriptbuilder.h>

#include <Angelscript/ScriptAPI/CASScheduler.h>
#include <Angelscript/ScriptAPI/Reflection/ASReflection.h>

#include <Angelscript/util/ASExtendAdapter.h>

#include <Angelscript/wrapper/ASCallable.h>

#include "Angelscript/ScriptAPI/Entities/ASCBaseEntity.h"
#include "Angelscript/ScriptAPI/CASEngine.h"
#include "Angelscript/ScriptAPI/ASCGlobalVars.h"
#include "Angelscript/ScriptAPI/ASGameRules.h"

#include "Angelscript/ScriptAPI/Extensions/CASGameRules.h"

#include "HLASConstants.h"

#include "CASMapModuleBuilder.h"

#include "CHLASManager.h"

CHLASManager g_ASManager;

class CHLASInitializer final : public IASInitializer
{
public:
	CHLASInitializer( CHLASManager& manager )
		: m_Manager( manager )
	{
	}

	bool GetMessageCallback( asSFuncPtr& outFuncPtr, void*& pOutObj, asDWORD& outCallConv ) override
	{
		outFuncPtr		= asMETHOD( CHLASManager, MessageCallback );
		pOutObj			= &m_Manager;
		outCallConv		= asCALL_THISCALL;

		return true;
	}

	bool RegisterCoreAPI( CASManager& manager ) override
	{
		Alert( at_console, "Angelscript v%s\nRegistering Angelscript API\n", ANGELSCRIPT_VERSION_STRING );

		RegisterStdString( manager.GetEngine() );
		RegisterScriptArray( manager.GetEngine(), true );
		RegisterScriptDictionary( manager.GetEngine() );
		RegisterScriptAny( manager.GetEngine() );
		RegisterScriptScheduler( manager.GetEngine() );
		RegisterScriptReflection( *manager.GetEngine() );

		return true;
	}

	bool AddHooks( CASManager& manager, CASHookManager& hookManager ) override
	{
		return true;
	}

	bool RegisterAPI( CASManager& manager ) override
	{
		auto& engine = *manager.GetEngine();

		RegisterScriptEntityDependencies( engine );
		RegisterScriptCBaseEntity( engine );
		RegisterScriptCEngine( engine );
		RegisterScriptCGlobalVars( engine );

		engine.RegisterInterface( "IGameRules" );

		RegisterScriptCGameRules( engine );
		RegisterScriptGameRules( engine );

		Alert( at_console, "Finished registering API\n%u Object types\n%u Global functions\n%u Global properties\n%u Enums\n%u Funcdefs\n%u Typedefs\n", 
			   engine.GetObjectTypeCount(), 
			   engine.GetGlobalFunctionCount(), engine.GetGlobalPropertyCount(), 
			   engine.GetEnumCount(), engine.GetFuncdefCount(), engine.GetTypedefCount() );

		return true;
	}

private:
	CHLASManager& m_Manager;
};

void CHLASManager::MessageCallback( asSMessageInfo* pMsg )
{
	const char* pType = "";

	//Get the prefix.
	switch( pMsg->type )
	{
	case asMSGTYPE_ERROR:	pType = "Error: "; break;
	case asMSGTYPE_WARNING: pType = "Warning: "; break;
	default: break;
	}

	//Only display the section if it was actually set. Some messages are not triggered by script code compilation or execution.
	const bool bHasSection = pMsg->section && *pMsg->section;

	bool bNeedsNewline = false;

	if( bHasSection )
	{
		Alert( at_console, "Section \"%s\"", pMsg->section );
		bNeedsNewline = true;
	}

	//Some messages don't refer to script code, and set both to 0.
	if( pMsg->row != 0 && pMsg->col != 0 )
	{
		if( bHasSection )
			Alert( at_console, " " );

		Alert( at_console, "(%d, %d)", pMsg->row, pMsg->col );
		bNeedsNewline = true;
	}

	if( bNeedsNewline )
		Alert( at_console, "\n" );

	Alert( at_console, "%s%s\n", pType, pMsg->message );
}

bool CHLASManager::Initialize()
{
	CHLASInitializer initializer( *this );

	if( !m_Manager.Initialize( initializer ) )
	{
		ALERT( at_console, "Failed to initialize Angelscript\n" );
		return false;
	}

	//Map scripts are per-map scripts that always have their hooks executed before any other module.
	auto descriptor = m_Manager.GetModuleManager().AddDescriptor( "MapScript", ModuleAccessMask::MAPSCRIPT, as::ModulePriority::HIGHEST );

	if( !descriptor.first )
	{
		ALERT( at_console, "Failed to add MapScript module type\n" );
		return false;
	}

	return true;
}

void CHLASManager::Shutdown()
{
	if( m_pModule )
	{
		m_Manager.GetModuleManager().RemoveModule( m_pModule );
		m_pModule = nullptr;
	}

	m_Manager.Shutdown();
}

CGameRules* CHLASManager::CreateGameRules()
{
	if( !m_pModule )
		return nullptr;

	//Because this is an extension class, and we don't have any way to create those properly yet, ask for the classname and create it.
	if( auto pFunction = m_pModule->GetModule()->GetFunctionByDecl( "bool InstallGameRules(string& out)" ) )
	{
		CASOwningContext ctx( *m_Manager.GetEngine() );

		CASFunction function( *pFunction, ctx );

		std::string szName;

		if( function.Call( CallFlag::NONE, &szName ) )
		{
			bool bResult = false;

			if( function.GetReturnValue( &bResult ) && bResult )
			{
				return as::CreateExtensionClassInstance<CASGameRules>( *m_Manager.GetEngine(), *m_pModule->GetModule(), szName.c_str(), "CGameRules", "GameRules" );
			}
		}
	}

	return nullptr;
}

void CHLASManager::WorldCreated( const char* const pszMapScriptFileName )
{
	CASMapModuleBuilder builder( pszMapScriptFileName );

	m_pModule = m_Manager.GetModuleManager().BuildModule( "MapScript", "MapModule", builder );

	if( m_pModule )
	{
		//Call MapInit if it exists.
		if( auto pFunction = m_pModule->GetModule()->GetFunctionByDecl( "void MapInit()" ) )
		{
			as::Call( pFunction );
		}
	}
	else
	{
		ALERT( at_console, "Failed to create map script\n" );
	}
}

void CHLASManager::WorldActivated()
{
	if( m_pModule )
	{
		//Call MapActivate if it exists.
		if( auto pFunction = m_pModule->GetModule()->GetFunctionByDecl( "void MapActivate()" ) )
		{
			as::Call( pFunction );
		}
	}
}

void CHLASManager::WorldEnded()
{
	if( m_pModule )
	{
		//Call MapShutdown if it exists.
		if( auto pFunction = m_pModule->GetModule()->GetFunctionByDecl( "void MapShutdown()" ) )
		{
			as::Call( pFunction );
		}

		m_Manager.GetModuleManager().RemoveModule( m_pModule );
		m_pModule = nullptr;
	}
}