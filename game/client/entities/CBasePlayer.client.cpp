#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "entities/CBasePlayer.h"

extern bool g_brunninggausspred;

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

void CBasePlayer::TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType )
{
}

int CBasePlayer::TakeDamage( CBaseEntity* pInflictor, CBaseEntity* pAttacker, float flDamage, int bitsDamageType )
{
	return 0;
}

/*
=====================
CBasePlayer::Killed

=====================
*/
void CBasePlayer::Killed( entvars_t *pevAttacker, GibAction gibAction )
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

bool CBasePlayer::SwitchWeapon( CBasePlayerItem *pWeapon )
{
	return false;
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

bool CBasePlayer::AddPlayerItem( CBasePlayerItem *pItem )
{
	return false;
}

bool CBasePlayer::RemovePlayerItem( CBasePlayerItem *pItem )
{
	return false;
}

void CBasePlayer::DropPlayerItem( char *pszItemName )
{
}

bool CBasePlayer::HasPlayerItem( CBasePlayerItem *pCheckItem ) const
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

	CBasePlayerItem *pTemp = m_pActiveItem;
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

	CBasePlayerItem *pItem = NULL;

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

void CBasePlayer::SetSuitUpdate( char *name, const SuitUpdateType updateType, int iNoRepeatTime )
{
}

void CBasePlayer::UpdateGeigerCounter( void )
{
}

void CBasePlayer::CheckTimeBasedDamage()
{
}

bool CBasePlayer::FBecomeProne()
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