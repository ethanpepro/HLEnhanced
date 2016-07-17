#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CBaseTrigger.h"

LINK_ENTITY_TO_CLASS( trigger, CBaseTrigger );

void CBaseTrigger::TeleportTouch( CBaseEntity *pOther )
{
	entvars_t* pevToucher = pOther->pev;
	edict_t	*pentTarget = NULL;

	// Only teleport monsters or clients
	if( !FBitSet( pevToucher->flags, FL_CLIENT | FL_MONSTER ) )
		return;

	if( !UTIL_IsMasterTriggered( m_sMaster, pOther ) )
		return;

	if( !( pev->spawnflags & SF_TRIGGER_ALLOWMONSTERS ) )
	{// no monsters allowed!
		if( FBitSet( pevToucher->flags, FL_MONSTER ) )
		{
			return;
		}
	}

	if( ( pev->spawnflags & SF_TRIGGER_NOCLIENTS ) )
	{// no clients allowed
		if( pOther->IsPlayer() )
		{
			return;
		}
	}

	pentTarget = FIND_ENTITY_BY_TARGETNAME( pentTarget, STRING( pev->target ) );
	if( FNullEnt( pentTarget ) )
		return;

	Vector tmp = VARS( pentTarget )->origin;

	if( pOther->IsPlayer() )
	{
		tmp.z -= pOther->pev->mins.z;// make origin adjustments in case the teleportee is a player. (origin in center, not at feet)
	}

	tmp.z++;

	pevToucher->flags &= ~FL_ONGROUND;

	UTIL_SetOrigin( pOther, tmp );

	pevToucher->angles = pentTarget->v.angles;

	if( pOther->IsPlayer() )
	{
		pevToucher->v_angle = pentTarget->v.angles;
	}

	pevToucher->fixangle = FIXANGLE_SET;
	pevToucher->velocity = pevToucher->basevelocity = g_vecZero;
}

void CBaseTrigger::MultiTouch( CBaseEntity *pOther )
{
	entvars_t	*pevToucher;

	pevToucher = pOther->pev;

	// Only touch clients, monsters, or pushables (depending on flags)
	if( ( ( pevToucher->flags & FL_CLIENT ) && !( pev->spawnflags & SF_TRIGGER_NOCLIENTS ) ) ||
		( ( pevToucher->flags & FL_MONSTER ) && ( pev->spawnflags & SF_TRIGGER_ALLOWMONSTERS ) ) ||
		( pev->spawnflags & SF_TRIGGER_PUSHABLES ) && FClassnameIs( pevToucher, "func_pushable" ) )
	{

#if 0
		// if the trigger has an angles field, check player's facing direction
		if( pev->movedir != g_vecZero )
		{
			UTIL_MakeVectors( pevToucher->angles );
			if( DotProduct( gpGlobals->v_forward, pev->movedir ) < 0 )
				return;         // not facing the right way
		}
#endif

		ActivateMultiTrigger( pOther );
	}
}

//
// the trigger was just touched/killed/used
// self.enemy should be set to the activator so it can be held through a delay
// so wait for the delay time before firing
//
void CBaseTrigger::ActivateMultiTrigger( CBaseEntity *pActivator )
{
	if( pev->nextthink > gpGlobals->time )
		return;         // still waiting for reset time

	if( !UTIL_IsMasterTriggered( m_sMaster, pActivator ) )
		return;

	if( FClassnameIs( pev, "trigger_secret" ) )
	{
		if( pev->enemy == NULL || !FClassnameIs( pev->enemy, "player" ) )
			return;
		gpGlobals->found_secrets++;
	}

	if( !FStringNull( pev->noise ) )
		EMIT_SOUND( ENT( pev ), CHAN_VOICE, ( char* ) STRING( pev->noise ), 1, ATTN_NORM );

	// don't trigger again until reset
	// pev->takedamage = DAMAGE_NO;

	m_hActivator = pActivator;
	SUB_UseTargets( m_hActivator, USE_TOGGLE, 0 );

	if( pev->message && pActivator->IsPlayer() )
	{
		UTIL_ShowMessage( STRING( pev->message ), pActivator );
		//		CLIENT_PRINTF( ENT( pActivator->pev ), print_center, STRING(pev->message) );
	}

	if( m_flWait > 0 )
	{
		SetThink( &CBaseTrigger::MultiWaitOver );
		pev->nextthink = gpGlobals->time + m_flWait;
	}
	else
	{
		// we can't just remove (self) here, because this is a touch function
		// called while C code is looping through area links...
		SetTouch( NULL );
		pev->nextthink = gpGlobals->time + 0.1;
		SetThink( &CBaseTrigger::SUB_Remove );
	}
}

// the wait time has passed, so set back up for another activation
void CBaseTrigger::MultiWaitOver( void )
{
	//	if (pev->max_health)
	//		{
	//		pev->health		= pev->max_health;
	//		pev->takedamage	= DAMAGE_YES;
	//		pev->solid		= SOLID_BBOX;
	//		}
	SetThink( NULL );
}

void CBaseTrigger::CounterUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	m_cTriggersLeft--;
	m_hActivator = pActivator;

	if( m_cTriggersLeft < 0 )
		return;

	const bool fTellActivator =
		( m_hActivator != nullptr ) &&
		FClassnameIs( m_hActivator->pev, "player" ) &&
		!FBitSet( pev->spawnflags, SPAWNFLAG_NOMESSAGE );

	if( m_cTriggersLeft != 0 )
	{
		if( fTellActivator )
		{
			// UNDONE: I don't think we want these Quakesque messages
			switch( m_cTriggersLeft )
			{
			case 1:		ALERT( at_console, "Only 1 more to go..." );		break;
			case 2:		ALERT( at_console, "Only 2 more to go..." );		break;
			case 3:		ALERT( at_console, "Only 3 more to go..." );		break;
			default:	ALERT( at_console, "There are more to go..." );	break;
			}
		}
		return;
	}

	// !!!UNDONE: I don't think we want these Quakesque messages
	if( fTellActivator )
		ALERT( at_console, "Sequence completed!" );

	ActivateMultiTrigger( m_hActivator );
}

//
// Cache user-entity-field values until spawn is called.
//
void CBaseTrigger::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "damage" ) )
	{
		pev->dmg = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "count" ) )
	{
		m_cTriggersLeft = ( int ) atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "damagetype" ) )
	{
		m_bitsDamageInflict = atoi( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseToggle::KeyValue( pkvd );
}

//
// ToggleUse - If this is the USE function for a trigger, its state will toggle every time it's fired
//
void CBaseTrigger::ToggleUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( pev->solid == SOLID_NOT )
	{// if the trigger is off, turn it on
		pev->solid = SOLID_TRIGGER;

		// Force retouch
		gpGlobals->force_retouch++;
	}
	else
	{// turn the trigger off
		pev->solid = SOLID_NOT;
	}
	UTIL_SetOrigin( this, pev->origin );
}

/*
================
InitTrigger
================
*/
void CBaseTrigger::InitTrigger()
{
	// trigger angles are used for one-way touches.  An angle of 0 is assumed
	// to mean no restrictions, so use a yaw of 360 instead.
	if( pev->angles != g_vecZero )
		SetMovedir( this );
	pev->solid = SOLID_TRIGGER;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL( ENT( pev ), STRING( pev->model ) );    // set size and link into world
	if( CVAR_GET_FLOAT( "showtriggers" ) == 0 )
		SetBits( pev->effects, EF_NODRAW );
}