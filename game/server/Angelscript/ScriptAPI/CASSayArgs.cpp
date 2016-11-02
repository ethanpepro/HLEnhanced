#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "client.h"

#include "Console/CASCommand.h"

#include "CASSayArgs.h"

CASSayArgs::CASSayArgs( CBasePlayer* pPlayer, const SayType::SayType sayType, const char* pszString )
	: m_pPlayer( pPlayer )
	, m_SayType( sayType )
	, m_szCommand( pszString )
	//Use CCommand to split the string properly
	, m_pArguments( new CASCommand( m_szCommand.c_str() ) )
{
}

CASSayArgs::~CASSayArgs()
{
	m_pArguments->Release();
}

void CASSayArgs::Release() const
{
	if( InternalRelease() )
		delete this;
}

const CASCommand* CASSayArgs::GetArguments() const
{
	m_pArguments->AddRef();

	return m_pArguments;
}

static void RegisterScriptSayType( asIScriptEngine& engine )
{
	const char* const pszObjectName = "SayType";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "SAY", SayType::SAY );
	engine.RegisterEnumValue( pszObjectName, "SAY_TEAM", SayType::SAY_TEAM );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

void RegisterScriptCSayArgs( asIScriptEngine& engine )
{
	RegisterScriptSayType( engine );

	const char* const pszObjectName = "CSayArgs";

	engine.RegisterObjectType(
		pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASSayArgs>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "CBasePlayer@ GetPlayer() const",
		asMETHOD( CASSayArgs, GetPlayer ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "SayType::SayType GetSayType() const",
		asMETHOD( CASSayArgs, GetSayType ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const string& GetCommand() const",
		asMETHOD( CASSayArgs, GetCommand ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "const CCommand@ GetArguments() const",
		asMETHOD( CASSayArgs, GetArguments ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool get_ShouldHide() const",
		asMETHOD( CASSayArgs, ShouldHide ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void set_ShouldHide(bool bShouldHide)", 
		asMETHOD( CASSayArgs, SetShouldHide ), asCALL_THISCALL );
}
