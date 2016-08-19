#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "CBasePlayer.h"

extern bool g_brunninggausspred;

void CBasePlayer::InitialSpawn()
{
	//Null this out so it doesn't refer to freed memory on map change - Solokiller
	m_pActiveItem = nullptr;
}

/*
=====================
CBasePlayer::Spawn

=====================
*/
void CBasePlayer::Spawn( void )
{
	if( m_pActiveItem )
		m_pActiveItem->Deploy();

	g_brunninggausspred = false;
}

void CBasePlayer::Jump()
{
}

void CBasePlayer::Duck()
{
}

void CBasePlayer::PreThink( void )
{
}

void CBasePlayer::PostThink()
{
}

Vector CBasePlayer::GetGunPosition( void )
{
	return g_vecZero;
}

float CBasePlayer::GiveHealth( float flHealth, int bitsDamageType )
{
	return 0;
}

void CBasePlayer::TraceAttack( const CTakeDamageInfo& info, Vector vecDir, TraceResult *ptr )
{
}

void CBasePlayer::OnTakeDamage( const CTakeDamageInfo& info )
{
}

/*
=====================
CBasePlayer::Killed

=====================
*/
void CBasePlayer::Killed( const CTakeDamageInfo& info, GibAction gibAction )
{
	// Holster weapon immediately, to allow it to cleanup
	if( m_pActiveItem )
		m_pActiveItem->Holster();

	g_brunninggausspred = false;
}

const char* CBasePlayer::TeamID() const
{
	return "";
}

bool CBasePlayer::Restore( CRestore& restore )
{
	return false;
}

void CBasePlayer::RenewItems( void )
{
}

void CBasePlayer::PackDeadPlayerItems( void )
{
}

void CBasePlayer::RemoveAllItems( const bool removeSuit )
{
}

bool CBasePlayer::SwitchWeapon( CBasePlayerWeapon *pWeapon )
{
	return false;
}

void CBasePlayer::TabulateAmmo()
{
}

void CBasePlayer::UpdateClientData( void )
{
}

void CBasePlayer::Precache( void )
{
}

bool CBasePlayer::IsOnLadder() const
{
	return false;
}

bool CBasePlayer::FlashlightIsOn() const
{
	return false;
}

void CBasePlayer::FlashlightTurnOn( void )
{
}

void CBasePlayer::FlashlightTurnOff( void )
{
}

void CBasePlayer::UpdatePlayerSound( void )
{
}

void CBasePlayer::DeathSound( void )
{
}

int CBasePlayer::Classify( void )
{
	return 0;
}

void CBasePlayer::SetAnimation( PLAYER_ANIM playerAnim )
{
}

void CBasePlayer::ImpulseCommands()
{
}

void CBasePlayer::CheatImpulseCommands( int iImpulse )
{
}

void CBasePlayer::StartDeathCam( void )
{
}

void CBasePlayer::StartObserver( Vector vecPosition, Vector vecViewAngle )
{
}

void CBasePlayer::AddPoints( int score, const bool bAllowNegativeScore )
{
}

void CBasePlayer::AddPointsToTeam( int score, const bool bAllowNegativeScore )
{
}

bool CBasePlayer::AddPlayerItem( CBasePlayerWeapon* pItem )
{
	ASSERT( pItem );

	CBasePlayerWeapon* pInsert = m_rgpPlayerItems[ pItem->iItemSlot() ];

	while( pInsert )
	{
		if( pInsert->ClassnameIs( pItem->GetClassname() ) )
		{
			return false;
		}
		pInsert = pInsert->m_pNext;
	}

	pItem->m_pNext = m_rgpPlayerItems[ pItem->iItemSlot() ];
	m_rgpPlayerItems[ pItem->iItemSlot() ] = pItem;

	return true;
}

bool CBasePlayer::RemovePlayerItem( CBasePlayerWeapon* pItem )
{
	ASSERT( pItem );

	CBasePlayerWeapon* pPrev = m_rgpPlayerItems[ pItem->iItemSlot() ];

	if( pPrev == pItem )
	{
		m_rgpPlayerItems[ pItem->iItemSlot() ] = pItem->m_pNext;
		return true;
	}
	else
	{
		while( pPrev && pPrev->m_pNext != pItem )
		{
			pPrev = pPrev->m_pNext;
		}

		if( pPrev )
		{
			pPrev->m_pNext = pItem->m_pNext;
			return true;
		}
	}

	return false;
}

void CBasePlayer::DropPlayerItem( char *pszItemName )
{
}

bool CBasePlayer::HasPlayerItem( CBasePlayerWeapon *pCheckItem ) const
{
	return false;
}

bool CBasePlayer::HasWeapons() const
{
	return false;
}

void CBasePlayer::SelectPrevItem( int iItem )
{
}

void CBasePlayer::SelectNextItem( int iItem )
{
}

/*
=====================
CBasePlayer::SelectLastItem

=====================
*/
void CBasePlayer::SelectLastItem( void )
{
	if( !m_pLastItem )
	{
		return;
	}

	if( m_pActiveItem && !m_pActiveItem->CanHolster() )
	{
		return;
	}

	if( m_pActiveItem )
		m_pActiveItem->Holster();

	CBasePlayerWeapon *pTemp = m_pActiveItem;
	m_pActiveItem = m_pLastItem;
	m_pLastItem = pTemp;
	m_pActiveItem->Deploy();
}

/*
=====================
CBasePlayer::SelectItem

Switch weapons
=====================
*/
void CBasePlayer::SelectItem( const char *pstr )
{
	if( !pstr )
		return;

	CBasePlayerWeapon *pItem = NULL;

	if( !pItem )
		return;


	if( pItem == m_pActiveItem )
		return;

	if( m_pActiveItem )
		m_pActiveItem->Holster();

	m_pLastItem = m_pActiveItem;
	m_pActiveItem = pItem;

	if( m_pActiveItem )
	{
		m_pActiveItem->Deploy();
	}
}

void CBasePlayer::ItemPreFrame()
{
}

void CBasePlayer::ItemPostFrame()
{
}

void CBasePlayer::EnableControl( const bool fControl )
{
}

int CBasePlayer::GiveAmmo( int iCount, const char *szName )
{
	return 0;
}

void CBasePlayer::SendAmmoUpdate( void )
{
}

void CBasePlayer::WaterMove()
{
}

void CBasePlayer::PlayerDeathThink( void )
{
}

void CBasePlayer::PlayerUse( void )
{
}

void CBasePlayer::CheckSuitUpdate()
{
}

void CBasePlayer::SetSuitUpdate( const char* const pszName, const SuitUpdateType updateType, int iNoRepeatTime )
{
}

void CBasePlayer::UpdateGeigerCounter( void )
{
}

void CBasePlayer::CheckTimeBasedDamage()
{
}

bool CBasePlayer::BarnacleVictimGrabbed( CBaseMonster* pBarnacle )
{
	return true;
}

void CBasePlayer::BarnacleVictimBitten( CBaseEntity* pBarnacle )
{
}

void CBasePlayer::BarnacleVictimReleased( void )
{
}

int CBasePlayer::GetAmmoIndex( const char *psz )
{
	return -1;
}

int CBasePlayer::AmmoInventory( int iAmmoIndex )
{
	return -1;
}

int CBasePlayer::Illumination() const
{
	return 0;
}

void CBasePlayer::ResetAutoaim()
{
}

Vector CBasePlayer::GetAutoaimVector( float flDelta )
{
	return g_vecZero;
}

Vector CBasePlayer::AutoaimDeflection( Vector &vecSrc, float flDist, float flDelta )
{
	return g_vecZero;
}

void CBasePlayer::ForceClientDllUpdate( void )
{
}

void CBasePlayer::SetCustomDecalFrames( int nFrames )
{
}

int CBasePlayer::GetCustomDecalFrames( void )
{
	return -1;
}