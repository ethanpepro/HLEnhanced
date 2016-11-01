#include <string>

#include <angelscript.h>

#include <Angelscript/util/ASUtil.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "Angelscript/HLASConstants.h"

#include "StringUtils.h"

#include "ASCServerEngine.h"

static void RegisterScriptForceType( asIScriptEngine& engine )
{
	const char* const pszObjectName = "ForceType";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "EXACTFILE", force_exactfile );
	engine.RegisterEnumValue( pszObjectName, "MODEL_SAMEBOUNDS", force_model_samebounds );
	engine.RegisterEnumValue( pszObjectName, "MODEL_SPECIFYBOUNDS", force_model_specifybounds );
	engine.RegisterEnumValue( pszObjectName, "MODEL_SPECIFYBOUNDS_IF_AVAIL", force_model_specifybounds_if_avail );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static int CServerEngine_PrecacheModel( enginefuncs_t* pThis, const std::string& szModelName )
{
	return pThis->pfnPrecacheModel( STRING( ALLOC_STRING( szModelName.c_str() ) ) );
}

static int CServerEngine_PrecacheGeneric( enginefuncs_t* pThis, const std::string& szFileName )
{
	return pThis->pfnPrecacheGeneric( STRING( ALLOC_STRING( szFileName.c_str() ) ) );
}

static int CServerEngine_ModelIndex( enginefuncs_t* pThis, const std::string& szModelName )
{
	return pThis->pfnModelIndex( szModelName.c_str() );
}

static int CServerEngine_ModelFrames( enginefuncs_t* pThis, int iModelIndex )
{
	return pThis->pfnModelFrames( iModelIndex );
}

static void CServerEngine_ChangeLevel( enginefuncs_t* pThis, const std::string& szLevelName )
{
	pThis->pfnChangeLevel( szLevelName.c_str(), nullptr );
}

static void CServerEngine_ChangeLevel2( enginefuncs_t* pThis, const std::string& szLevelName, const std::string& szLandmarkName )
{
	pThis->pfnChangeLevel( szLevelName.c_str(), szLandmarkName.c_str() );
}

static void CServerEngine_ServerCommand( enginefuncs_t* pThis, const std::string& szCommand )
{
	std::string szActualCommand = szCommand;

	Trim( szActualCommand );

	if( szActualCommand.empty() )
		return;

	const char cEnd = szActualCommand.back();

	//The engine only accepts commands if they end with either of these, so make it easy on script writers and do it for them. - Solokiller
	if( cEnd != '\n' && cEnd != ';' )
	{
		szActualCommand += '\n';
	}

	pThis->pfnServerCommand( szActualCommand.c_str() );
}

static void CServerEngine_ServerExecute( enginefuncs_t* pThis )
{
	pThis->pfnServerExecute();
}

static void CServerEngine_ClientCommand( asIScriptGeneric* pArguments )
{
	auto pClient = reinterpret_cast<CBaseEntity*>( pArguments->GetArgAddress( 0 ) );

	if( !pClient )
	{
		Alert( at_warning, "CServerEngine::ClientCommand: Got null client entity!\n" );
		return;
	}

	const std::string& szFormat = *reinterpret_cast<const std::string*>( pArguments->GetArgAddress( 1 ) );

	char szBuffer[ 4096 ];

	if( as::SPrintf( szBuffer, szFormat.c_str(), 2, *pArguments ) )
	{
		g_engfuncs.pfnClientCommand( pClient->edict(), "%s", szBuffer );
	}
}

static void CServerEngine_ServerPrint( enginefuncs_t* pThis, const std::string& szString )
{
	pThis->pfnServerPrint( szString.c_str() );
}

static void CServerEngine_ServerPrintf( asIScriptGeneric* pArguments )
{
	const std::string& szFormat = *reinterpret_cast<const std::string*>( pArguments->GetArgAddress( 0 ) );

	char szBuffer[ 4096 ];

	if( as::SPrintf( szBuffer, szFormat.c_str(), 1, *pArguments ) )
	{
		g_engfuncs.pfnServerPrint( szBuffer );
	}
}

static bool CServerEngine_IsMapValid( enginefuncs_t* pThis, const std::string& szFilename )
{
	return !!pThis->pfnIsMapValid( szFilename.c_str() );
}

static bool CServerEngine_IsDedicatedServer( const enginefuncs_t* pThis )
{
	return !!pThis->pfnIsDedicatedServer();
}

static int CServerEngine_GetCurrentPlayer( const enginefuncs_t* pThis )
{
	return pThis->pfnGetCurrentPlayer();
}

static void CServerEngine_ForceUnmodifiedFile( enginefuncs_t* pThis, const std::string& szFilename )
{
	pThis->pfnForceUnmodified( force_exactfile, nullptr, nullptr, STRING( ALLOC_STRING( szFilename.c_str() ) ) );
}

static void CServerEngine_ForceUnmodifiedModel( enginefuncs_t* pThis, FORCE_TYPE forceType, const std::string& szFilename, const Vector& vecMins = g_vecZero, const Vector& vecMaxs = g_vecZero )
{
	if( forceType == force_exactfile )
	{
		//exactfile doesn't need bounds, but it still copies the data. This makes it slightly more efficient.
		pThis->pfnForceUnmodified( forceType, nullptr, nullptr, STRING( ALLOC_STRING( szFilename.c_str() ) ) );
	}
	else
	{
		pThis->pfnForceUnmodified( forceType, vecMins, vecMaxs, STRING( ALLOC_STRING( szFilename.c_str() ) ) );
	}
}

static int CServerEngine_CheckParm( const enginefuncs_t* pThis, const std::string& szCmdLineToken )
{
	return pThis->pfnCheckParm( szCmdLineToken.c_str(), nullptr );
}

static int CServerEngine_CheckParmToken( const enginefuncs_t* pThis, const std::string& szCmdLineToken, std::string& szNextToken, bool& bHasNextToken )
{
	char* pszNextToken = nullptr;

	const int iResult = pThis->pfnCheckParm( szCmdLineToken.c_str(), &pszNextToken );

	if( pszNextToken )
		szNextToken = pszNextToken;
	else
		szNextToken = "";

	bHasNextToken = pszNextToken != nullptr;

	return iResult;
}

static int CServerEngine_NumberOfEntities( const enginefuncs_t* pThis )
{
	return pThis->pfnNumberOfEntities();
}

#define AS_CSERVERENGINE_NAME "CServerEngine"

void RegisterScriptCServerEngine( asIScriptEngine& engine )
{
	RegisterScriptForceType( engine );

	const char* const pszObjectName = AS_CSERVERENGINE_NAME;

	engine.RegisterObjectType(
		pszObjectName, 0, asOBJ_REF | asOBJ_NOCOUNT );

	engine.RegisterObjectMethod(
		pszObjectName, "int PrecacheModel(const string& in szModelName)",
		asFUNCTION( CServerEngine_PrecacheModel ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int PrecacheGeneric(const string& in szFileName)",
		asFUNCTION( CServerEngine_PrecacheGeneric ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int ModelIndex(const string& in szModelName)",
		asFUNCTION( CServerEngine_ModelIndex ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int ModelFrames(int iModelIndex)",
		asFUNCTION( CServerEngine_ModelFrames ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ChangeLevel(const string& in szLevelName)",
		asFUNCTION( CServerEngine_ChangeLevel ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ChangeLevel(const string& in szLevelName, const string& in szLandmarkName)",
		asFUNCTION( CServerEngine_ChangeLevel2 ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ServerCommand(const string& in szCommand)",
		asFUNCTION( CServerEngine_ServerCommand ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ServerExecute()",
		asFUNCTION( CServerEngine_ServerExecute ), asCALL_CDECL_OBJFIRST );

	as::RegisterVarArgsMethod(
		engine, pszObjectName, "void", "ClientCommand", "CBaseEntity@ pClient, const string& in szFormat",
		1, AS_MAX_VARARGS, asFUNCTION( CServerEngine_ClientCommand ) );

	engine.RegisterObjectMethod(
		pszObjectName, "void ServerPrint(const string& in szString)",
		asFUNCTION( CServerEngine_ServerPrint ), asCALL_CDECL_OBJFIRST );

	as::RegisterVarArgsMethod(
		engine, pszObjectName, "void", "ServerPrint", "const string& in szFormat", 
		1, AS_MAX_VARARGS, asFUNCTION( CServerEngine_ServerPrintf ) );

	engine.RegisterObjectMethod(
		pszObjectName, "int NumberOfEntities() const",
		asFUNCTION( CServerEngine_NumberOfEntities ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsMapValid(const string& in szFileName)",
		asFUNCTION( CServerEngine_IsMapValid ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "bool IsDedicatedServer() const",
		asFUNCTION( CServerEngine_IsDedicatedServer ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int GetCurrentPlayer() const",
		asFUNCTION( CServerEngine_GetCurrentPlayer ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ForceUnmodified(const string& in szFilename)",
		asFUNCTION( CServerEngine_ForceUnmodifiedFile ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "void ForceUnmodified(ForceType::ForceType forceType, const string& in szFilename, const Vector& in vecMins = g_vecZero, const Vector& in vecMaxs = g_vecZero)",
		asFUNCTION( CServerEngine_ForceUnmodifiedModel ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int CheckParm(const string& in szCmdLineToken) const",
		asFUNCTION( CServerEngine_CheckParm ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "int CheckParm(const string& in szCmdLineToken, string& out szNextToken, bool& out bHasNextToken) const",
		asFUNCTION( CServerEngine_CheckParmToken ), asCALL_CDECL_OBJFIRST );

	engine.RegisterGlobalProperty( AS_CSERVERENGINE_NAME " g_engfuncs", &g_engfuncs );
}
