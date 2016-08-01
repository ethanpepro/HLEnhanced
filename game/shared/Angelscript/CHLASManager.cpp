#include <string>

#include "extdll.h"
#include "util.h"

#include "Angelscript/ScriptAPI/Extensions/CASGameRules.h"

#include "CHLASManager.h"

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

bool CHLASManager::InitializeManager( IASInitializer& initializer )
{
	if( !m_Manager.Initialize( initializer ) )
	{
		ALERT( at_console, "Failed to initialize Angelscript\n" );
		return false;
	}

	return true;
}

void CHLASManager::Shutdown()
{
	m_Manager.Shutdown();
}