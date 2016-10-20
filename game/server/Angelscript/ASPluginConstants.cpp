#include <cstring>

#include "extdll.h"
#include "util.h"

#include "ASPluginConstants.h"

PluginLifetime PluginLifetimeFromString( const char* const pszString )
{
	if( pszString && stricmp( pszString, "HOTRELOADABLE" ) != 0 )
	{
		if( stricmp( pszString, "MAP" ) == 0 )
			return PluginLifetime::MAP;
		else if( stricmp( pszString, "SERVER" ) == 0 )
			return PluginLifetime::SERVER;
	}

	return PluginLifetime::HOTRELOADABLE;
}

const char* PluginLifetimeToString( const PluginLifetime lifetime )
{
	switch( lifetime )
	{
	case PluginLifetime::HOTRELOADABLE:	return "HOTRELOADABLE";
	case PluginLifetime::MAP:			return "MAP";
	case PluginLifetime::SERVER:		return "SERVER";
	}

	ASSERT( !"Unhandled plugin lifetime constant" );

	return "UNKNOWN";
}
