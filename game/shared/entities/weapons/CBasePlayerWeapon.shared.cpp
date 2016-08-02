/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CBasePlayer.h"
#include "Weapons.h"

#include "CWeaponInfoCache.h"

#ifdef CLIENT_DLL
#include "hud.h"
#include "cl_util.h"

#include "com_weapons.h"
#endif

void CBasePlayerWeapon::Precache()
{
	BaseClass::Precache();

	m_pWeaponInfo = g_WeaponInfoCache.LoadWeaponInfo( m_iId, GetClassname() );

	m_iDefaultAmmo = GetWeaponInfo()->GetDefaultAmmo();
}

bool CBasePlayerWeapon::PlayEmptySound()
{
	if( m_bPlayEmptySound )
	{
#ifdef CLIENT_DLL
		HUD_PlaySound( "weapons/357_cock1.wav", 0.8 );
#else
		EMIT_SOUND( m_pPlayer, CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORM );
#endif
		m_bPlayEmptySound = false;
		return false;
	}
	return false;
}

void CBasePlayerWeapon::ResetEmptySound()
{
	m_bPlayEmptySound = true;
}

bool CBasePlayerWeapon::CanDeploy() const
{
	bool bHasAmmo = false;

	if( !pszAmmo1() )
	{
		// this weapon doesn't use ammo, can always deploy.
		return true;
	}

	if( pszAmmo1() )
	{
		bHasAmmo |= ( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] != 0 );
	}
	if( pszAmmo2() )
	{
		bHasAmmo |= ( m_pPlayer->m_rgAmmo[ SecondaryAmmoIndex() ] != 0 );
	}
	if( m_iClip > 0 )
	{
		bHasAmmo |= 1;
	}
	if( !bHasAmmo )
	{
		return false;
	}

	return true;
}

bool CBasePlayerWeapon::DefaultReload( int iAnim, float fDelay, int body )
{
	if( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] <= 0 )
		return false;

	int j = min( GetWeaponInfo()->GetMaxMagazine() - m_iClip, m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] );

	if( j == 0 )
		return false;

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + fDelay;

#ifdef SERVER_DLL
	//The server doesn't pass this, so always 0 it out. - Solokiller
	//TODO: why? - Solokiller
	body = 0;
#endif

	//!!UNDONE -- reload sound goes here !!!
	SendWeaponAnim( iAnim, UseDecrement(), body );

	m_fInReload = true;

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3;
	return true;
}

void CBasePlayerWeapon::GetWeaponData( weapon_data_t& data )
{
	data.m_iClip = m_iClip;

	data.m_flTimeWeaponIdle = max( m_flTimeWeaponIdle, -0.001f );
	data.m_flNextPrimaryAttack = max( m_flNextPrimaryAttack, -0.001f );
	data.m_flNextSecondaryAttack = max( m_flNextSecondaryAttack, -0.001f );

	data.m_fInReload = m_fInReload;

	data.fuser1 = max( pev->fuser1, -0.001f );
}

void CBasePlayerWeapon::SetWeaponData( const weapon_data_t& data )
{
	m_iClip = data.m_iClip;

	m_flTimeWeaponIdle = data.m_flTimeWeaponIdle;
	m_flNextPrimaryAttack = data.m_flNextPrimaryAttack;
	m_flNextSecondaryAttack = data.m_flNextSecondaryAttack;

	m_fInReload = data.m_fInReload != 0;

	pev->fuser1 = data.fuser1;
}

int CBasePlayerWeapon::PrimaryAmmoIndex() const
{
	return GetWeaponInfo()->GetPrimaryAmmo() ? GetWeaponInfo()->GetPrimaryAmmo()->GetID() : WEAPON_NOCLIP;
}

int CBasePlayerWeapon::SecondaryAmmoIndex() const
{
	//Used to return -1 unconditionally. - Solokiller
	return GetWeaponInfo()->GetSecondaryAmmo() ? GetWeaponInfo()->GetSecondaryAmmo()->GetID() : WEAPON_NOCLIP;
}