#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "CWeaponInfoCache.h"
#include "CWeaponInfo.h"

#include "CBasePlayerItem.h"

CBasePlayerItem::CBasePlayerItem( const int iID )
	: m_iId( iID )
{
}

void CBasePlayerItem::Precache()
{
	BaseClass::Precache();

	m_pWeaponInfo = g_WeaponInfoCache.LoadWeaponInfo( m_iId, GetClassname() );
}

int CBasePlayerItem::PrimaryAmmoIndex() const
{
	return GetWeaponInfo()->GetPrimaryAmmo() ? GetWeaponInfo()->GetPrimaryAmmo()->GetID() : WEAPON_NOCLIP;
}

int CBasePlayerItem::SecondaryAmmoIndex() const
{
	//Used to return -1 unconditionally. - Solokiller
	return GetWeaponInfo()->GetSecondaryAmmo() ? GetWeaponInfo()->GetSecondaryAmmo()->GetID() : WEAPON_NOCLIP;
}