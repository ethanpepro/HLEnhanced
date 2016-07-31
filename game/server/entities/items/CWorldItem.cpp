#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CWorldItem.h"

LINK_ENTITY_TO_CLASS( world_items, CWorldItem );

void CWorldItem::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "type" ) )
	{
		m_iType = atoi( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseEntity::KeyValue( pkvd );
}

void CWorldItem::Spawn( void )
{
	CBaseEntity *pEntity = NULL;

	switch( m_iType )
	{
	case 44: // ITEM_BATTERY:
		pEntity = CBaseEntity::Create( "item_battery", GetAbsOrigin(), pev->angles );
		break;
	case 42: // ITEM_ANTIDOTE:
		pEntity = CBaseEntity::Create( "item_antidote", GetAbsOrigin(), pev->angles );
		break;
	case 43: // ITEM_SECURITY:
		pEntity = CBaseEntity::Create( "item_security", GetAbsOrigin(), pev->angles );
		break;
	case 45: // ITEM_SUIT:
		pEntity = CBaseEntity::Create( "item_suit", GetAbsOrigin(), pev->angles );
		break;
	}

	if( !pEntity )
	{
		ALERT( at_console, "unable to create world_item %d\n", m_iType );
	}
	else
	{
		pEntity->pev->target = pev->target;
		pEntity->pev->targetname = pev->targetname;
		pEntity->pev->spawnflags = pev->spawnflags;
	}

	UTIL_RemoveNow( this );
}