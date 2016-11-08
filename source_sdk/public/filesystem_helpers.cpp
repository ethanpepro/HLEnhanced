//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#if !defined(_STATIC_LINKED) || defined(_SHARED_LIB)

#include <cwchar>
#include <experimental/filesystem>

#include "filesystem.h"
#include "filesystem_helpers.h"
#include "characterset.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// wordbreak parsing set
static characterset_t	g_BreakSet, g_BreakSetIncludingColons;

static void InitializeCharacterSets()
{
	static bool s_CharacterSetInitialized = false;
	if (!s_CharacterSetInitialized)
	{
		CharacterSetBuild( &g_BreakSet, "{}()'" );
		CharacterSetBuild( &g_BreakSetIncludingColons, "{}()':" );
		s_CharacterSetInitialized = true;
	}
}


const char* ParseFile( const char* pFileBytes, char* pToken, bool* pWasQuoted, characterset_t *pCharSet )
{
	if (pWasQuoted)
		*pWasQuoted = false;

	if (!pFileBytes)
		return 0;

	InitializeCharacterSets();

	// YWB:  Ignore colons as token separators in COM_Parse
	static bool com_ignorecolons = false;  
	characterset_t& breaks = pCharSet ? *pCharSet : (com_ignorecolons ? g_BreakSet : g_BreakSetIncludingColons);
	
	int c;
	int len = 0;
	pToken[0] = 0;
	
// skip whitespace
skipwhite:

	while ( (c = *pFileBytes) <= ' ')
	{
		if (c == 0)
			return 0;                    // end of file;
		pFileBytes++;
	}
	
// skip // comments
	if (c=='/' && pFileBytes[1] == '/')
	{
		while (*pFileBytes && *pFileBytes != '\n')
			pFileBytes++;
		goto skipwhite;
	}
	
// skip c-style comments
	if (c=='/' && pFileBytes[1] == '*' )
	{
		// Skip "/*"
		pFileBytes += 2;

		while ( *pFileBytes  )
		{
			if ( *pFileBytes == '*' &&
				 pFileBytes[1] == '/' )
			{
				pFileBytes += 2;
				break;
			}

			pFileBytes++;
		}

		goto skipwhite;
	}

// handle quoted strings specially
	if (c == '\"')
	{
		if (pWasQuoted)
			*pWasQuoted = true;

		pFileBytes++;
		while (1)
		{
			c = *pFileBytes++;
			if (c=='\"' || !c)
			{
				pToken[len] = 0;
				return pFileBytes;
			}
			pToken[len] = c;
			len++;
		}
	}

// parse single characters
	if ( IN_CHARACTERSET( breaks, c ) )
	{
		pToken[len] = c;
		len++;
		pToken[len] = 0;
		return pFileBytes+1;
	}

// parse a regular word
	do
	{
		pToken[len] = c;
		pFileBytes++;
		len++;
		c = *pFileBytes;
		if ( IN_CHARACTERSET( breaks, c ) )
			break;
	} while (c>32);
	
	pToken[len] = 0;
	return pFileBytes;
}


char* ParseFile( char* pFileBytes, char* pToken, bool* pWasQuoted )
{
	return (char*)ParseFile( (const char*)pFileBytes, pToken, pWasQuoted );
}

bool FS_GetFileTypeForFullPath( char const *pFullPath, wchar_t *buf, size_t bufSizeInBytes )
{
	if( !pFullPath )
		return false;
	
	bufSizeInBytes /= sizeof( wchar_t );
	
	if( !buf || bufSizeInBytes == 0 )
		return false;
	
	std::experimental::filesystem::path path( pFullPath );
	
	auto szExt = path.extension().wstring();
	
	wcsncpy( buf, szExt.c_str(), bufSizeInBytes );
	buf[ bufSizeInBytes - 1 ] = L'\0';
	
	return true;
}

#endif // !_STATIC_LINKED || _SHARED_LIB


