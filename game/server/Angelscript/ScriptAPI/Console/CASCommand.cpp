#include <angelscript.h>

#include "CASCommand.h"

CASCommand::CASCommand( const int iArgc, char** ppArgv )
	: m_Command( iArgc, ppArgv )
{
}

CASCommand::CASCommand( const char* pszCommand )
	: m_Command( pszCommand )
{
}

CASCommand::CASCommand( const char* pszCommand, const char* pszValue )
	: m_Command( pszCommand, pszValue )
{
}

CASCommand::CASCommand( const CCommand& other )
	: m_Command( other )
{
}

CASCommand& CASCommand::operator=( const CCommand& other )
{
	m_Command = other;

	return *this;
}

CASCommand::~CASCommand()
{
}

void RegisterScriptCCommand( asIScriptEngine& engine )
{
	const char* pszObjectName = "CCommand";

	engine.RegisterObjectType( 
		pszObjectName, 0, asOBJ_REF );

	as::RegisterRefCountedBaseClass<CASCommand>( &engine, pszObjectName );

	engine.RegisterObjectMethod(
		pszObjectName, "int ArgC() const", 
		asMETHOD( CASCommand, ArgC ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetCommandString() const", 
		asMETHOD( CASCommand, GetCommandString ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string GetArgumentsString() const", 
		asMETHOD( CASCommand, GetArgumentsString ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string Arg(const int iIndex) const", 
		asMETHOD( CASCommand, Arg ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string opIndex(const int iIndex) const", 
		asMETHOD( CASCommand, Arg ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "string FindArg(const string& in szArgument) const", 
		asMETHOD( CASCommand, FindArg ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int FindIntArg(const string& in szArgument, const int iDefault = 0) const", 
		asMETHOD( CASCommand, FindIntArg ), asCALL_THISCALL );
}
