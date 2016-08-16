#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"
#include "CBasePlayer.h"

#include "CMap.h"

BEGIN_DATADESC_NOBASE( CMap )
	DEFINE_FIELD( m_flPrevFrameTime, FIELD_TIME ),
	DEFINE_FIELD( m_bUseCustomHudColors, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_flLastHudColorChangeTime, FIELD_TIME ),
	DEFINE_FIELD( m_HudColors.m_PrimaryColor, FIELD_INTEGER ),
	DEFINE_FIELD( m_HudColors.m_EmptyItemColor, FIELD_INTEGER ),
	DEFINE_FIELD( m_HudColors.m_AmmoBarColor, FIELD_INTEGER ),
END_DATADESC()

CMap* CMap::m_pInstance = nullptr;

CMap* CMap::CreateInstance()
{
	ASSERT( !m_pInstance );

	m_pInstance = new CMap();

	m_pInstance->Create();

	return m_pInstance;
}

CMap* CMap::GetInstance()
{
	ASSERT( m_pInstance );

	return m_pInstance;
}

void CMap::DestroyInstance()
{
	ASSERT( m_pInstance );

	delete m_pInstance;

	m_pInstance = nullptr;
}

bool CMap::Exists()
{
	return m_pInstance != nullptr;
}

CMap* CMap::CreateIfNeeded()
{
	if( !Exists() )
		CreateInstance();

	return GetInstance();
}

CMap::CMap()
{
}

CMap::~CMap()
{
}

bool CMap::Save( CSave& save )
{
	auto pDataMap = GetDataMap();

	return save.WriteFields( "CMap", this, pDataMap->pTypeDesc, pDataMap->uiNumDescriptors );
}

bool CMap::Restore( CRestore& restore )
{
	auto pDataMap = GetDataMap();

	return restore.ReadFields( "CMap", this, pDataMap->pTypeDesc, pDataMap->uiNumDescriptors );
}

void CMap::Create()
{
	//Ammo types have to be registered ahead of time in all cases.
	RegisterAmmoTypes();

	ResetHudColors();
}

void CMap::Think()
{
	CBasePlayer* pPlayer;

	for( int iPlayer = 1; iPlayer <= gpGlobals->maxClients; ++iPlayer )
	{
		pPlayer = UTIL_PlayerByIndex( iPlayer );

		if( !pPlayer || !pPlayer->IsConnected() )
			continue;

		SendHudColors( pPlayer );
	}

	m_flPrevFrameTime = gpGlobals->time;
}

void CMap::SendHudColors( CBasePlayer* pPlayer, const bool bForce )
{
	if( !pPlayer->UsesCustomHudColors() )
	{
		//Not using per-player settings and last global change time was after last think, send changes.
		//Players may have reset their color; signalled by UsesCustomHudColors returning false and GetLastHudColorChangeTime being >= m_flPrevFrameTime.
		if( bForce || ( m_flLastHudColorChangeTime >= m_flPrevFrameTime ) || ( pPlayer->GetLastHudColorChangeTime() >= m_flPrevFrameTime ) )
			SendHudColors( pPlayer, m_HudColors );
	}
	else
	{
		//Using per-player settings and last change time was after last think, send changes.
		if( bForce || (pPlayer->GetLastHudColorChangeTime() >= m_flPrevFrameTime ) )
			SendHudColors( pPlayer, pPlayer->GetHudColors() );
	}
}

void CMap::SendHudColors( CBasePlayer* pPlayer, const CHudColors& colors )
{
	MESSAGE_BEGIN( MSG_ONE, gmsgHudColors, nullptr, pPlayer );
		WRITE_BYTE( colors.m_PrimaryColor.r );
		WRITE_BYTE( colors.m_PrimaryColor.g );
		WRITE_BYTE( colors.m_PrimaryColor.b );

		WRITE_BYTE( colors.m_EmptyItemColor.r );
		WRITE_BYTE( colors.m_EmptyItemColor.g );
		WRITE_BYTE( colors.m_EmptyItemColor.b );

		WRITE_BYTE( colors.m_AmmoBarColor.r );
		WRITE_BYTE( colors.m_AmmoBarColor.g );
		WRITE_BYTE( colors.m_AmmoBarColor.b );
	MESSAGE_END();
}

void CMap::ResetHudColors()
{
	m_bUseCustomHudColors = false;

	m_flLastHudColorChangeTime = gpGlobals->time;

	m_HudColors.Reset();
}

void CMap::LoadGlobalModelReplacement( const char* const pszFileName )
{
	ASSERT( pszFileName );

	if( m_pGlobalModelReplacement = m_ModelReplacement.AcquireMap( pszFileName) )
	{
		Alert( at_console, "Global Model Replacement file \"%s\" loaded\n", pszFileName );
	}
}
