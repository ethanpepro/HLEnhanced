#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "entities/CBasePlayer.h"
#include "weapons.h"

#include "CItem.h"

#include "CItemSecurity.h"

LINK_ENTITY_TO_CLASS( item_security, CItemSecurity );

void CItemSecurity::Spawn( void )
{
	Precache();
	SET_MODEL( ENT( pev ), "models/w_security.mdl" );
	CItem::Spawn();
}

void CItemSecurity::Precache( void )
{
	PRECACHE_MODEL( "models/w_security.mdl" );
}

bool CItemSecurity::MyTouch( CBasePlayer *pPlayer )
{
	pPlayer->m_rgItems[ ITEM_SECURITY ] += 1;
	return true;
}