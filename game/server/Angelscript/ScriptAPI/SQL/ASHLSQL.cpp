#include <cstdarg>
#include <memory>
#include <string>

#include <angelscript.h>

#include <Angelscript/ScriptAPI/ASCDateTime.h>
#include <Angelscript/ScriptAPI/ASCTime.h>

#include <Angelscript/ScriptAPI/SQL/CASSQLThreadPool.h>

#include <Angelscript/ScriptAPI/SQL/SQLite/ASSQLite.h>
#include <Angelscript/ScriptAPI/SQL/SQLite/CASSQLiteConnection.h>

#include <Angelscript/ScriptAPI/SQL/MySQL/ASMySQL.h>
#include <Angelscript/ScriptAPI/SQL/MySQL/CASMySQLConnection.h>

#include "extdll.h"
#include "util.h"
#include "CFile.h"
#include "Server.h"

#include "ASHLSQL.h"

#define SQLITE_BASE_DIR "scripts/databases"
#define SQLITE_EXT ".sqlite"

#define MYSQL_DEFAULT_CONN_BLOCK "default_mysql_connection"

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

CASSQLThreadPool* g_pSQLThreadPool = nullptr;

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

static unsigned int ParseMySQLPort( std::string& szHostName )
{
	//Based on AMX's SQLX interface; allow scripts to specify a port using host:port format. - Solokiller
	size_t uiPortSep = szHostName.find( ':' );

	unsigned int uiPort = 3306;

	if( uiPortSep != std::string::npos )
	{
		uiPort = strtoul( &szHostName[ uiPortSep + 1 ], nullptr, 10 );

		//Trim the port part from the string.
		szHostName.resize( uiPortSep );
	}

	return uiPort;
}

static CASMySQLConnection* HLCreateMySQLConnection( const std::string& szHost, const std::string& szUser, const std::string& szPassword, const std::string& szDatabase = "" )
{
	std::string szHostName = szHost;

	const unsigned int uiPort = ParseMySQLPort( szHostName );

	return new CASMySQLConnection( *g_pSQLThreadPool, szHostName.c_str(), szUser.c_str(), szPassword.c_str(), szDatabase.c_str(), uiPort, "", 0 );
}

/**
*	Creates a MySQL connection using default connection settings defined in the server's MySQL config.
*/
static CASMySQLConnection* HLCreateMySQLConnectionWithDefaults( const std::string& szDatabase = "" )
{
	if( !( *as_mysql_config.string ) )
	{
		Alert( at_logged, "SQL::CreateMySQLConnectionWithDefaults: No config file specified; cannot create connection\n" );
		return nullptr;
	}

	//Parse the config file.
	//Only load from the mod directory to prevent malicious servers from downloading files and overriding them.
	CFile file( as_mysql_config.string, "r", "GAMECONFIG" );

	if( !file.IsOpen() )
	{
		Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Couldn't open config file \"%s\"!\n", as_mysql_config.string );
		return nullptr;
	}

	const size_t uiSize = file.Size();

	auto buffer = std::make_unique<char[]>( uiSize + 1 );

	file.Read( buffer.get(), uiSize );

	buffer[ uiSize ] = '\0';

	file.Close();

	const char* pszData = buffer.get();

	pszData = COM_Parse( pszData );

	if( !pszData )
	{
		Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Couldn't parse config file \"%s\"!\n", as_mysql_config.string );
		return nullptr;
	}

	if( strcmp( MYSQL_DEFAULT_CONN_BLOCK, com_token ) )
	{
		Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Expected \"" MYSQL_DEFAULT_CONN_BLOCK "\", got \"%s\"\n", com_token );
		return nullptr;
	}

	pszData = COM_Parse( pszData );

	if( !pszData )
	{
		Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Couldn't parse config file \"%s\"!\n", as_mysql_config.string );
		return nullptr;
	}

	if( strcmp( "{", com_token ) )
	{
		Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Expected \"{\", got \"%s\"\n", com_token );
		return nullptr;
	}

	char szKey[ 512 ];

	char szHost[ 512 ] = {};
	char szUser[ 512 ] = {};
	char szPass[ 1024 ] = {};

	//Password can be empty, so track separately.
	bool bHasPass = false;

	while( true )
	{
		pszData = COM_Parse( pszData );

		if( !pszData )
		{
			Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Couldn't parse config file \"%s\"!\n", as_mysql_config.string );
			return nullptr;
		}

		if( strcmp( "}", com_token ) == 0 )
		{
			break;
		}

		strncpy( szKey, com_token, sizeof( szKey ) );
		szKey[ sizeof( szKey ) - 1 ] = '\0';

		pszData = COM_Parse( pszData );

		if( !pszData )
		{
			Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Couldn't parse config file \"%s\"!\n", as_mysql_config.string );
			return nullptr;
		}

		if( strcmp( "host", szKey ) == 0 )
		{
			strncpy( szHost, com_token, sizeof( szHost ) );
			szHost[ sizeof( szHost ) - 1 ] = '\0';
		}
		else if( strcmp( "user", szKey ) == 0 )
		{
			strncpy( szUser, com_token, sizeof( szUser ) );
			szUser[ sizeof( szUser ) - 1 ] = '\0';
		}
		else if( strcmp( "pass", szKey ) == 0 )
		{
			strncpy( szPass, com_token, sizeof( szPass ) );
			szPass[ sizeof( szPass ) - 1 ] = '\0';

			bHasPass = true;
		}
		else
		{
			//Don't output the value, it might contain sensitive data.
			Alert( at_warning, "SQL::CreateMySQLConnectionWithDefaults: Unknown keyvalue \"%s\"\n", szKey );
		}
	}

	bool bHasAllValues = true;

	if( !( *szHost ) )
	{
		Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Missing value for key \"host\"\n" );
		bHasAllValues = false;
	}

	if( !( *szUser ) )
	{
		Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Missing value for key \"user\"\n" );
		bHasAllValues = false;
	}

	if( !bHasPass )
	{
		Alert( at_error, "SQL::CreateMySQLConnectionWithDefaults: Missing value for key \"pass\"\n" );
		bHasAllValues = false;
	}

	if( !bHasAllValues )
		return nullptr;

	std::string szHostName = szHost;

	const unsigned int uiPort = ParseMySQLPort( szHostName );

	return new CASMySQLConnection( *g_pSQLThreadPool, szHostName.c_str(), szUser, szPass, szDatabase.c_str(), uiPort, "", 0 );
}

void RegisterScriptHLSQL( asIScriptEngine& engine )
{
	g_pSQLThreadPool = new CASSQLThreadPool( CalculateThreadCount(), &::SQLLogFunc );

	//Call an SQLite function to load the library. - Solokiller
	Alert( at_console, "SQLite library version: %s\n", sqlite3_libversion() );

	//Call a MySQL function to load the library. - Solokiller
	Alert( at_console, "MariaDB library version: %s\n", mysql_get_client_info() );

	RegisterScriptCTime( engine );
	RegisterScriptCDateTime( engine );

	RegisterScriptSQLCommon( engine );
	RegisterScriptSQLite( engine );
	RegisterScriptMySQL( engine );

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "SQL" );

	engine.RegisterGlobalFunction(
		"SQLiteConnection@ CreateSQLiteConnection(const string& in szDatabase)",
		asFUNCTION( HLCreateSQLiteConnection ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"MySQLConnection@ CreateMySQLConnection(const string& in szHost, const string& in szUser, const string& in szPassword, const string& in szDatabase = \"\")",
		asFUNCTION( HLCreateMySQLConnection ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"MySQLConnection@ CreateMySQLConnectionWithDefaults(const string& in szDatabase = \"\")",
		asFUNCTION( HLCreateMySQLConnectionWithDefaults ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}