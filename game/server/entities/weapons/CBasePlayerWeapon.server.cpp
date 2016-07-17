#include "extdll.h"
#include "util.h"
#include "gamerules/GameRules.h"
#include "cbase.h"
#include "entities/CBasePlayer.h"
#include "Weapons.h"

#include "entities/weapons/CBasePlayerWeapon.h"

extern int gmsgCurWeapon;

bool CBasePlayerWeapon::AddToPlayer( CBasePlayer *pPlayer )
{
	const bool bResult = CBasePlayerItem::AddToPlayer( pPlayer );

	pPlayer->pev->weapons |= ( 1 << m_iId );

	if( !m_iPrimaryAmmoType )
	{
		m_iPrimaryAmmoType = pPlayer->GetAmmoIndex( pszAmmo1() );
		m_iSecondaryAmmoType = pPlayer->GetAmmoIndex( pszAmmo2() );
	}


	if( bResult )
		return AddWeapon();
	return false;
}

// CALLED THROUGH the newly-touched weapon's instance. The existing player weapon is pOriginal
bool CBasePlayerWeapon::AddDuplicate( CBasePlayerItem *pOriginal )
{
	if( m_iDefaultAmmo )
	{
		return ExtractAmmo( ( CBasePlayerWeapon * ) pOriginal );
	}
	else
	{
		// a dead player dropped this.
		return ExtractClipAmmo( ( CBasePlayerWeapon * ) pOriginal );
	}
}

//=========================================================
// called by the new item with the existing item as parameter
//
// if we call ExtractAmmo(), it's because the player is picking up this type of weapon for 
// the first time. If it is spawned by the world, m_iDefaultAmmo will have a default ammo amount in it.
// if  this is a weapon dropped by a dying player, has 0 m_iDefaultAmmo, which means only the ammo in 
// the weapon clip comes along. 
//=========================================================
bool CBasePlayerWeapon::ExtractAmmo( CBasePlayerWeapon *pWeapon )
{
	bool bReturn = false;

	if( pszAmmo1() != NULL )
	{
		// blindly call with m_iDefaultAmmo. It's either going to be a value or zero. If it is zero,
		// we only get the ammo in the weapon's clip, which is what we want. 
		bReturn = pWeapon->AddPrimaryAmmo( m_iDefaultAmmo, ( char * ) pszAmmo1(), iMaxClip() );
		m_iDefaultAmmo = 0;
	}

	if( pszAmmo2() != NULL )
	{
		bReturn = pWeapon->AddSecondaryAmmo( 0, pszAmmo2() );
	}

	return bReturn;
}

//=========================================================
// called by the new item's class with the existing item as parameter
//=========================================================
bool CBasePlayerWeapon::ExtractClipAmmo( CBasePlayerWeapon *pWeapon )
{
	int			iAmmo;

	if( m_iClip == WEAPON_NOCLIP )
	{
		iAmmo = 0;// guns with no clips always come empty if they are second-hand
	}
	else
	{
		iAmmo = m_iClip;
	}

	//Used to return the return value directly, but this was never 0. It's supposed to return true if ammo is allowed to be added (even if no ammo was actually added). - Solokiller
	return pWeapon->m_pPlayer->GiveAmmo( iAmmo, ( char * ) pszAmmo1() ) != -1; // , &m_iPrimaryAmmoType
}

bool CBasePlayerWeapon::AddPrimaryAmmo( int iCount, const char *szName, int iMaxClip )
{
	int iIdAmmo;

	if( iMaxClip < 1 )
	{
		m_iClip = -1;
		iIdAmmo = m_pPlayer->GiveAmmo( iCount, szName );
	}
	else if( m_iClip == 0 )
	{
		int i;
		i = min( m_iClip + iCount, iMaxClip ) - m_iClip;
		m_iClip += i;
		iIdAmmo = m_pPlayer->GiveAmmo( iCount - i, szName );
	}
	else
	{
		iIdAmmo = m_pPlayer->GiveAmmo( iCount, szName );
	}

	// m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = iMaxCarry; // hack for testing

	if( iIdAmmo > 0 )
	{
		m_iPrimaryAmmoType = iIdAmmo;
		if( m_pPlayer->HasPlayerItem( this ) )
		{
			// play the "got ammo" sound only if we gave some ammo to a player that already had this gun.
			// if the player is just getting this gun for the first time, DefaultTouch will play the "picked up gun" sound for us.
			EMIT_SOUND( ENT( pev ), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM );
		}
	}

	return iIdAmmo > 0;
}


bool CBasePlayerWeapon::AddSecondaryAmmo( int iCount, const char *szName )
{
	int iIdAmmo;

	iIdAmmo = m_pPlayer->GiveAmmo( iCount, szName );

	//m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] = iMax; // hack for testing

	if( iIdAmmo > 0 )
	{
		m_iSecondaryAmmoType = iIdAmmo;
		EMIT_SOUND( ENT( pev ), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM );
	}
	return iIdAmmo > 0;
}

void CBasePlayerWeapon::SendWeaponAnim( int iAnim, int skiplocal, int body )
{
	if( UseDecrement() )
		skiplocal = 1;
	else
		skiplocal = 0;

	m_pPlayer->pev->weaponanim = iAnim;

#if defined( CLIENT_WEAPONS )
	if( skiplocal && ENGINE_CANSKIP( m_pPlayer->edict() ) )
		return;
#endif

	MESSAGE_BEGIN( MSG_ONE, SVC_WEAPONANIM, NULL, m_pPlayer->pev );
	WRITE_BYTE( iAnim );						// sequence number
	WRITE_BYTE( pev->body );					// weaponmodel bodygroup.
	MESSAGE_END();
}

//=========================================================
// IsUseable - this function determines whether or not a 
// weapon is useable by the player in its current state. 
// (does it have ammo loaded? do I have any ammo for the 
// weapon?, etc)
//=========================================================
bool CBasePlayerWeapon::IsUseable()
{
	if( m_iClip <= 0 )
	{
		if( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] <= 0 && ( m_pPrimaryAmmo && m_pPrimaryAmmo->GetMaxCarry() != -1 ) )
		{
			// clip is empty (or nonexistant) and the player has no more ammo of this type. 
			return false;
		}
	}

	return true;
}

bool CBasePlayerWeapon::DefaultDeploy( char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal /* = 0 */, int body )
{
	if( !CanDeploy() )
		return false;

	m_pPlayer->TabulateAmmo();
	m_pPlayer->pev->viewmodel = MAKE_STRING( szViewModel );
	m_pPlayer->pev->weaponmodel = MAKE_STRING( szWeaponModel );
	strcpy( m_pPlayer->m_szAnimExtention, szAnimExt );
	SendWeaponAnim( iAnim, skiplocal, body );

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;
	m_flLastFireTime = 0.0;

	return true;
}

bool CBasePlayerWeapon::DefaultReload( int iClipSize, int iAnim, float fDelay, int body )
{
	if( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] <= 0 )
		return false;

	int j = min( iClipSize - m_iClip, m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] );

	if( j == 0 )
		return false;

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + fDelay;

	//!!UNDONE -- reload sound goes here !!!
	SendWeaponAnim( iAnim, UseDecrement() ? 1 : 0 );

	m_fInReload = true;

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3;
	return true;
}

bool CanAttack( float attack_time, float curtime, const bool isPredicted )
{
#if defined( CLIENT_WEAPONS )
	if( !isPredicted )
#else
	if( 1 )
#endif
	{
		return attack_time <= curtime;
	}
	else
	{
		return attack_time <= 0.0;
	}
}

void CBasePlayerWeapon::ItemPostFrame( void )
{
	if( ( m_fInReload ) && ( m_pPlayer->m_flNextAttack <= UTIL_WeaponTimeBase() ) )
	{
		// complete the reload. 
		int j = min( iMaxClip() - m_iClip, m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] );

		// Add them to the clip
		m_iClip += j;
		m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] -= j;

		m_pPlayer->TabulateAmmo();

		m_fInReload = false;
	}

	if( !( m_pPlayer->pev->button & IN_ATTACK ) )
	{
		m_flLastFireTime = 0.0f;
	}

	if( ( m_pPlayer->pev->button & IN_ATTACK2 ) && CanAttack( m_flNextSecondaryAttack, gpGlobals->time, UseDecrement() ) )
	{
		if( pszAmmo2() && !m_pPlayer->m_rgAmmo[ SecondaryAmmoIndex() ] )
		{
			m_bFireOnEmpty = true;
		}

		m_pPlayer->TabulateAmmo();
		SecondaryAttack();
		m_pPlayer->pev->button &= ~IN_ATTACK2;
	}
	else if( ( m_pPlayer->pev->button & IN_ATTACK ) && CanAttack( m_flNextPrimaryAttack, gpGlobals->time, UseDecrement() ) )
	{
		if( ( m_iClip == 0 && pszAmmo1() ) || ( iMaxClip() == -1 && !m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] ) )
		{
			m_bFireOnEmpty = true;
		}

		m_pPlayer->TabulateAmmo();
		PrimaryAttack();
	}
	else if( m_pPlayer->pev->button & IN_RELOAD && iMaxClip() != WEAPON_NOCLIP && !m_fInReload )
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		Reload();
	}
	else if( !( m_pPlayer->pev->button & ( IN_ATTACK | IN_ATTACK2 ) ) )
	{
		// no fire buttons down

		m_bFireOnEmpty = false;

		if( !IsUseable() && m_flNextPrimaryAttack < ( UseDecrement() ? 0.0 : gpGlobals->time ) )
		{
			// weapon isn't useable, switch.
			if( !( iFlags() & ITEM_FLAG_NOAUTOSWITCHEMPTY ) && g_pGameRules->GetNextBestWeapon( m_pPlayer, this ) )
			{
				m_flNextPrimaryAttack = ( UseDecrement() ? 0.0 : gpGlobals->time ) + 0.3;
				return;
			}
		}
		else
		{
			// weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
			if( m_iClip == 0 && !( iFlags() & ITEM_FLAG_NOAUTORELOAD ) && m_flNextPrimaryAttack < ( UseDecrement() ? 0.0 : gpGlobals->time ) )
			{
				Reload();
				return;
			}
		}

		WeaponIdle();
		return;
	}

	// catch all
	if( ShouldWeaponIdle() )
	{
		WeaponIdle();
	}
}

bool CBasePlayerWeapon::UpdateClientData( CBasePlayer *pPlayer )
{
	bool bSend = false;
	int state = 0;
	if( pPlayer->m_pActiveItem == this )
	{
		if( pPlayer->m_fOnTarget )
			state = WEAPON_IS_ONTARGET;
		else
			state = 1;//TODO: define constant - Solokiller
	}

	// Forcing send of all data!
	if( !pPlayer->m_fWeapon )
	{
		bSend = true;
	}

	// This is the current or last weapon, so the state will need to be updated
	if( this == pPlayer->m_pActiveItem ||
		this == pPlayer->m_pClientActiveItem )
	{
		if( pPlayer->m_pActiveItem != pPlayer->m_pClientActiveItem )
		{
			bSend = true;
		}
	}

	// If the ammo, state, or fov has changed, update the weapon
	if( m_iClip != m_iClientClip ||
		state != m_iClientWeaponState ||
		pPlayer->m_iFOV != pPlayer->m_iClientFOV )
	{
		bSend = true;
	}

	if( bSend )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, pPlayer->pev );
		WRITE_BYTE( state );
		WRITE_BYTE( m_iId );
		WRITE_BYTE( m_iClip );
		MESSAGE_END();

		m_iClientClip = m_iClip;
		m_iClientWeaponState = state;
		pPlayer->m_fWeapon = true;
	}

	if( m_pNext )
		m_pNext->UpdateClientData( pPlayer );

	return true;
}

//=========================================================
// RetireWeapon - no more ammo for this gun, put it away.
//=========================================================
void CBasePlayerWeapon::RetireWeapon( void )
{
	// first, no viewmodel at all.
	m_pPlayer->pev->viewmodel = iStringNull;
	m_pPlayer->pev->weaponmodel = iStringNull;
	//m_pPlayer->pev->viewmodelindex = NULL;

	g_pGameRules->GetNextBestWeapon( m_pPlayer, this );
}

void CBasePlayerWeapon::Holster( int skiplocal /* = 0 */ )
{
	m_fInReload = false; // cancel any reload in progress.
	m_pPlayer->pev->viewmodel = 0;
	m_pPlayer->pev->weaponmodel = 0;
}

//=========================================================
//=========================================================
int CBasePlayerWeapon::PrimaryAmmoIndex() const
{
	return m_iPrimaryAmmoType;
}

//=========================================================
//=========================================================
int CBasePlayerWeapon::SecondaryAmmoIndex() const
{
	//Used to return -1 unconditionally. - Solokiller
	return m_iSecondaryAmmoType;
}

void CBasePlayerWeapon::PrintState( void )
{
	ALERT( at_console, "primary:  %f\n", m_flNextPrimaryAttack );
	ALERT( at_console, "idle   :  %f\n", m_flTimeWeaponIdle );

	//	ALERT( at_console, "nextrl :  %f\n", m_flNextReload );
	//	ALERT( at_console, "nextpum:  %f\n", m_flPumpTime );

	//	ALERT( at_console, "m_frt  :  %f\n", m_fReloadTime );
	ALERT( at_console, "m_finre:  %s\n", m_fInReload ? "true" : "false" );
	//	ALERT( at_console, "m_finsr:  %i\n", m_fInSpecialReload );

	ALERT( at_console, "m_iclip:  %i\n", m_iClip );
}

//=========================================================================
// GetNextAttackDelay - An accurate way of calcualting the next attack time.
//=========================================================================
float CBasePlayerWeapon::GetNextAttackDelay( float delay )
{
	if( m_flLastFireTime == 0 || m_flNextPrimaryAttack == -1 )
	{
		// At this point, we are assuming that the client has stopped firing
		// and we are going to reset our book keeping variables.
		m_flLastFireTime = gpGlobals->time;
		m_flPrevPrimaryAttack = delay;
	}
	// calculate the time between this shot and the previous
	float flTimeBetweenFires = gpGlobals->time - m_flLastFireTime;
	float flCreep = 0.0f;
	if( flTimeBetweenFires > 0 )
		flCreep = flTimeBetweenFires - m_flPrevPrimaryAttack; // postive or negative

															  // save the last fire time
	m_flLastFireTime = gpGlobals->time;

	float flNextAttack = UTIL_WeaponTimeBase() + delay - flCreep;
	// we need to remember what the m_flNextPrimaryAttack time is set to for each shot, 
	// store it as m_flPrevPrimaryAttack.
	m_flPrevPrimaryAttack = flNextAttack - UTIL_WeaponTimeBase();
	// 	char szMsg[256];
	// 	_snprintf( szMsg, sizeof(szMsg), "next attack time: %0.4f\n", gpGlobals->time + flNextAttack );
	// 	OutputDebugString( szMsg );
	return flNextAttack;
}