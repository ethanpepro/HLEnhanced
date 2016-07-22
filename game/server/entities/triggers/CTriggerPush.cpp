#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerPush.h"

LINK_ENTITY_TO_CLASS( trigger_push, CTriggerPush );

void CTriggerPush::Spawn()
{
	if( pev->angles == g_vecZero )
		pev->angles.y = 360;
	InitTrigger();

	if( pev->speed == 0 )
		pev->speed = 100;

	if( FBitSet( pev->spawnflags, SF_TRIGGER_PUSH_START_OFF ) )// if flagged to Start Turned Off, make trigger nonsolid.
		pev->solid = SOLID_NOT;

	SetUse( &CTriggerPush::ToggleUse );

	SetAbsOrigin( GetAbsOrigin() );		// Link into the list
}

void CTriggerPush::KeyValue( KeyValueData *pkvd )
{
	CBaseTrigger::KeyValue( pkvd );
}

void CTriggerPush::Touch( CBaseEntity *pOther )
{
	// UNDONE: Is there a better way than health to detect things that have physics? (clients/monsters)
	switch( pOther->pev->movetype )
	{
	case MOVETYPE_NONE:
	case MOVETYPE_PUSH:
	case MOVETYPE_NOCLIP:
	case MOVETYPE_FOLLOW:
		return;
	}

	if( pOther->pev->solid != SOLID_NOT && pOther->pev->solid != SOLID_BSP )
	{
		// Instant trigger, just transfer velocity and remove
		if( FBitSet( pev->spawnflags, SF_TRIG_PUSH_ONCE ) )
		{
			pOther->pev->velocity = pOther->pev->velocity + ( pev->speed * pev->movedir );
			if( pOther->pev->velocity.z > 0 )
				pOther->pev->flags &= ~FL_ONGROUND;
			UTIL_Remove( this );
		}
		else
		{	// Push field, transfer to base velocity
			Vector vecPush = ( pev->speed * pev->movedir );
			if( pOther->pev->flags & FL_BASEVELOCITY )
				vecPush = vecPush + pOther->pev->basevelocity;

			pOther->pev->basevelocity = vecPush;

			pOther->pev->flags |= FL_BASEVELOCITY;
			//			ALERT( at_console, "Vel %f, base %f\n", pevToucher->velocity.z, pevToucher->basevelocity.z );
		}
	}
}