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
#include "Weapons.h"
#include "CBasePlayer.h"

LINK_ENTITY_TO_CLASS( player, CBasePlayer );

void CBasePlayer::SetWeaponAnimType( const char* const pszExtension )
{
	ASSERT( pszExtension );

	strncpy( m_szAnimExtension, pszExtension, sizeof( m_szAnimExtension ) - 1 );
	m_szAnimExtension[ sizeof( m_szAnimExtension ) - 1 ] = '\0';
}

CBasePlayerWeapon* CBasePlayer::GetWeapon( int iBucket, int iPosition )
{
	ASSERT( iBucket >= 0 && iBucket < MAX_WEAPON_SLOTS );

	CBasePlayerWeapon* pWeapon = m_rgpPlayerItems[ iBucket ];

	while( pWeapon )
	{
		if( pWeapon->GetWeaponInfo()->GetPosition() == iPosition )
			return pWeapon;

		pWeapon = pWeapon->m_pNext;
	}

	return nullptr;
}
