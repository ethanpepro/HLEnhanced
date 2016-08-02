#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "CWeaponInfo.h"

#ifdef CLIENT_DLL
#include "hud.h"
#include "CWeaponHUDInfo.h"
#endif

#include "CWeaponInfoCache.h"

CWeaponInfoCache g_WeaponInfoCache;

const char* const CWeaponInfoCache::WEAPON_INFO_DIR = "weapon_info";

const CWeaponInfo* CWeaponInfoCache::FindWeaponInfo( const char* const pszWeaponName ) const
{
	ASSERT( pszWeaponName );

	if( !pszWeaponName )
		return nullptr;

	auto it = m_Infos.find( pszWeaponName );

	if( it != m_Infos.end() )
		return it->second.get();

	return nullptr;
}

const CWeaponInfo* CWeaponInfoCache::LoadWeaponInfo( const int iID, const char* const pszWeaponName, const char* const pszSubDir )
{
	ASSERT( pszWeaponName );
	ASSERT( *pszWeaponName );

	if( auto pInfo = FindWeaponInfo( pszWeaponName ) )
		return pInfo;

	std::unique_ptr<CWeaponInfo> info = std::make_unique<CWeaponInfo>();

	if( LoadWeaponInfoFromFile( pszWeaponName, pszSubDir, *info ) )
	{
		CWeaponInfo* pInfo = info.get();

		auto result = m_Infos.insert( std::make_pair( pInfo->GetWeaponName(), std::move( info ) ) );

		if( result.second )
		{
			pInfo->SetID( iID );

#ifdef CLIENT_DLL
			//Load HUD info right away.
			CWeaponHUDInfo* pHUDInfo = new CWeaponHUDInfo();

			if( !pHUDInfo->LoadFromFile( pszWeaponName ) )
			{
				Alert( at_warning, "CWeaponInfoCache::LoadWeaponInfo: Couldn't load weapon \"%s\" HUD info\n", pszWeaponName );
			}

			pInfo->SetHUDInfo( pHUDInfo );
#endif

			return pInfo;
		}

		Alert( at_error, "CWeaponInfoCache::LoadWeaponInfo: Failed to insert weapon info \"%s\" into cache!\n", pszWeaponName );
	}

	return &m_DefaultInfo;
}

void CWeaponInfoCache::ClearInfos()
{
	m_Infos.clear();
}

void CWeaponInfoCache::EnumInfos( EnumInfoCallback pCallback, void* pUserData ) const
{
	ASSERT( pCallback );

	if( !pCallback )
		return;

	for( const auto& info : m_Infos )
	{
		if( !pCallback( *info.second, pUserData ) )
			break;
	}
}

bool CWeaponInfoCache::LoadWeaponInfoFromFile( const char* const pszWeaponName, const char* const pszSubDir, CWeaponInfo& info )
{
	char szPath[ MAX_PATH ] = {};

	int iResult;
	
	if( pszSubDir )
	{
		iResult = snprintf( szPath, sizeof( szPath ), "%s/%s/%s.txt", WEAPON_INFO_DIR, pszSubDir, pszWeaponName );
	}
	else
	{
		iResult = snprintf( szPath, sizeof( szPath ), "%s/%s.txt", WEAPON_INFO_DIR, pszWeaponName );
	}

	if( iResult < 0 || static_cast<size_t>( iResult ) >= sizeof( szPath ) )
	{
		//Subdir can be null, that's expected. - Solokiller
		Alert( at_error, "CWeaponInfoCache::LoadWeaponInfoFromFile: Failed to format file path for \"%s\" (subdir: \"%s\")!\n", pszWeaponName, pszSubDir );
		return false;
	}

	FileHandle_t hFile = g_pFileSystem->Open( szPath, "r" );

	if( hFile == FILESYSTEM_INVALID_HANDLE )
	{
		Alert( at_error, "CWeaponInfoCache::LoadWeaponInfoFromFile: Failed to open file \"%s\"!\n", szPath );
		return false;
	}

	Alert( at_aiconsole, "CWeaponInfoCache::LoadWeaponInfoFromFile: Opened file \"%s\"\n", szPath );

	info.SetWeaponName( pszWeaponName );

	const size_t uiSize = g_pFileSystem->Size( hFile );

	auto buffer = std::make_unique<char[]>( uiSize + 1 );

	g_pFileSystem->Read( buffer.get(), uiSize, hFile );

	g_pFileSystem->Close( hFile );

	char* pszBuffer = buffer.get();

	pszBuffer = COM_Parse( pszBuffer );

	if( stricmp( com_token, pszWeaponName ) )
	{
		Alert( at_error, "CWeaponInfoCache::LoadWeaponInfoFromFile: Expected weapon name \"%s\", got \"%s\"\n", pszWeaponName, com_token );
		return false;
	}

	pszBuffer = COM_Parse( pszBuffer );

	if( com_token[ 0 ] != '{' )
	{
		Alert( at_error, "CWeaponInfoCache::LoadWeaponInfoFromFile: Expected token '{', got \"%s\"!\n", com_token );
		return false;
	}

	//Parse in all values.
	char szKey[ MAX_COM_TOKEN ];

	while( true )
	{
		pszBuffer = COM_Parse( pszBuffer );

		if( !pszBuffer )
		{
			Alert( at_error, "CWeaponInfoCache::LoadWeaponInfoFromFile: Unexpected EOF while reading from file (searching key)!\n" );
			return false;
		}

		if( com_token[ 0 ] == '}' )
		{
			break;
		}

		strncpy( szKey, com_token, sizeof( szKey ) );
		szKey[ sizeof( szKey ) - 1 ] = '\0';

		pszBuffer = COM_Parse( pszBuffer );

		if( !pszBuffer )
		{
			Alert( at_error, "CWeaponInfoCache::LoadWeaponInfoFromFile: Unexpected EOF while reading from file (searching value for \"%s\"!\n", szKey );
			return false;
		}

		if( com_token[ 0 ] == '}' )
		{
			break;
		}

		if( !info.KeyValue( szKey, com_token ) )
		{
			Alert( at_aiconsole, "CWeaponInfoCache::LoadWeaponInfoFromFile: Unhandled keyvalue \"%s\" \"%s\"\n", szKey, com_token );
		}
	}

	return true;
}