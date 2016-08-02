#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "CWeaponInfoCache.h"
#include "CWeaponInfo.h"

#include "CBasePlayerItem.h"

const char* ItemInfoFlagToString( const ItemInfoFlag flag )
{
	switch( flag )
	{
	case ITEM_FLAG_NONE:				return "NONE";
	case ITEM_FLAG_SELECTONEMPTY:		return "SELECTONEMPTY";
	case ITEM_FLAG_NOAUTORELOAD:		return "NOAUTORELOAD";
	case ITEM_FLAG_NOAUTOSWITCHEMPTY:	return "NOAUTOSWITCHEMPTY";
	case ITEM_FLAG_LIMITINWORLD:		return "LIMITINWORLD";
	case ITEM_FLAG_EXHAUSTIBLE:			return "EXHAUSTIBLE";
	}

	ASSERTSZ( false, "ItemInfoFlagToString: unknown constant!" );

	return "NONE";
}

ItemInfoFlag StringToItemInfoFlag( const char* const pszString )
{
	if( !pszString )
		return ITEM_FLAG_NONE;

	if( stricmp( pszString, "NONE" ) == 0 )
		return ITEM_FLAG_NONE;

	if( stricmp( pszString, "SELECTONEMPTY" ) == 0 )
		return ITEM_FLAG_SELECTONEMPTY;

	if( stricmp( pszString, "NOAUTORELOAD" ) == 0 )
		return ITEM_FLAG_NOAUTORELOAD;

	if( stricmp( pszString, "NOAUTOSWITCHEMPTY" ) == 0 )
		return ITEM_FLAG_NOAUTOSWITCHEMPTY;

	if( stricmp( pszString, "LIMITINWORLD" ) == 0 )
		return ITEM_FLAG_LIMITINWORLD;

	if( stricmp( pszString, "EXHAUSTIBLE" ) == 0 )
		return ITEM_FLAG_EXHAUSTIBLE;

	return ITEM_FLAG_NONE;
}

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