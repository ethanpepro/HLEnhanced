#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CONSOLE_CASCOMMAND_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CONSOLE_CASCOMMAND_H

#include <string>

#include <Angelscript/util/CASBaseClass.h>

#include "CCommand.h"

/*
* Command arguments
* This is a list of all commands passed in to the command that receives this
* Is reference counted to allow scripts to pass it to other functions without expensive copy construction
*/
class CASCommand : public CASRefCountedBaseClass
{
public:
	CASCommand( const int iArgc, char** ppArgv );
	CASCommand( const char* pszCommand );
	CASCommand( const char* pszCommand, const char* pszValue );
	CASCommand( const CCommand& other );
	CASCommand& operator=( const CCommand& other );
	~CASCommand();

	void Release() const
	{
		if( InternalRelease() )
			delete this;
	}

	int ArgC() const { return m_Command.ArgC(); }

	std::string GetCommandString() const { return m_Command.GetCommandString(); }

	std::string GetArgumentsString() const { return m_Command.GetArgumentsString(); }

	std::string Arg( const int iIndex ) const { return m_Command.Arg( iIndex ); }

	std::string FindArg( const std::string& szArgument ) const { return m_Command.FindArg( szArgument.c_str() ); }

	int FindIntArg( const std::string& szArgument, const int iDefault = 0 ) const { return m_Command.FindIntArg( szArgument.c_str(), iDefault ); }

private:
	CCommand m_Command;

private:
	CASCommand( const CASCommand& ) = delete;
	CASCommand& operator=( const CASCommand& ) = delete;
};

class asIScriptEngine;

/**
*	Registers the CASCommand class as the CCommand class.
*	@param engine Script engine.
*/
void RegisterScriptCCommand( asIScriptEngine& engine );

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CONSOLE_CASCOMMAND_H
