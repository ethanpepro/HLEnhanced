#include <cstdarg>
#include <string>

#include <angelscript.h>

#include <Angelscript/ScriptAPI/SQL/CASSQLThreadPool.h>
#include <Angelscript/ScriptAPI/SQL/SQLite/ASSQLite.h>
#include <Angelscript/ScriptAPI/SQL/SQLite/CASSQLiteConnection.h>
#include <Angelscript/ScriptAPI/SQL/MySQL/ASMySQL.h>

#include "extdll.h"
#include "util.h"

#include "ASHLSQL.h"

#define SQLITE_BASE_DIR "scripts/databases"
#define SQLITE_EXT ".sqlite"

static void SQLLogFunc( const char* const pszFormat, ... )
{
	char szBuffer[ 4096 ];

	va_list list;

	va_start( list, pszFormat );
	const int iResult = vsnprintf( szBuffer, sizeof( szBuffer ), pszFormat, list );
	va_end( list );

	if( iResult >= 0 || static_cast<size_t>( iResult ) < sizeof( szBuffer ) )
	{
		Alert( at_console, "SQL: %s", szBuffer );
	}
	else
	{
		ASSERT( false );
		Alert( at_console, "SQL: Log formatting failed\n" );
	}
}

static size_t CalculateThreadCount()
{
	const unsigned int uiThreads = std::thread::hardware_concurrency();

	//Only use 1 thread for now. - Solokiller
	if( uiThreads >= 1 )
		return 1;

	return 0;
}

static CASSQLThreadPool* g_pSQLThreadPool = nullptr;

static CASSQLiteConnection* HLCreateSQLiteConnection( const std::string& szDatabase )
{
	if( szDatabase.empty() )
	{
		Alert( at_error, "SQL::CreateSQLiteConnection: Empty database name!\n" );
		return nullptr;
	}

	g_pFileSystem->CreateDirHierarchy( SQLITE_BASE_DIR, nullptr );

	char szGameDir[ MAX_PATH ];
	char szFilename[ MAX_PATH ];

	if( !UTIL_GetGameDir( szGameDir, sizeof( szGameDir ) ) )
	{
		Alert( at_error, "SQL::CreateSQLiteConnection: Failed to get game directory!\n" );
		return nullptr;
	}

	const int iResult = snprintf( szFilename, sizeof( szFilename ), "%s/%s/%s%s", szGameDir, SQLITE_BASE_DIR, szDatabase.c_str(), SQLITE_EXT );
	
	if( iResult < 0 || static_cast<size_t>( iResult ) >= sizeof( szFilename ) )
	{
		Alert( at_error, "SQL::CreateSQLiteConnection: Failed to format database \"%s\" filename!\n", szDatabase.c_str() );
		return nullptr;
	}

	//TODO: filter access to databases here so only authorized access works - Solokiller

	return new CASSQLiteConnection( *g_pSQLThreadPool, szFilename );
}

void RegisterScriptHLSQL( asIScriptEngine& engine )
{
	g_pSQLThreadPool = new CASSQLThreadPool( CalculateThreadCount(), &::SQLLogFunc );

	RegisterScriptSQLite( engine );
	//RegisterScriptMySQL( engine );

	//Call an SQLite function to load the library. - Solokiller
	Alert( at_console, "SQLite library version: %s\n", sqlite3_libversion() );

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "SQL" );

	engine.RegisterGlobalFunction(
		"SQLiteConnection@ CreateSQLiteConnection(const string& in szDatabase)",
		asFUNCTION( HLCreateSQLiteConnection ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}