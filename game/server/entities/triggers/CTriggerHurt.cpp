#include "extdll.h"
#include "util.h"
#include "gamerules/GameRules.h"
#include "cbase.h"
#include "CBasePlayer.h"

#include "CTriggerHurt.h"

BEGIN_DATADESC( CTriggerHurt )
	DEFINE_TOUCHFUNC( HurtTouch ),
	DEFINE_THINKFUNC( RadiationThink ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_hurt, CTriggerHurt );

//
// trigger_hurt - hurts anything that touches it. if the trigger has a targetname, firing it will toggle state
//
//int gfToggleState = 0; // used to determine when all radiation trigger hurts have called 'RadiationThink'

void CTriggerHurt::Spawn( void )
{
	InitTrigger();
	SetTouch( &CTriggerHurt::HurtTouch );

	if( HasTargetname() )
	{
		SetUse( &CTriggerHurt::ToggleUse );
	}
	else
	{
		SetUse( NULL );
	}

	if( m_bitsDamageInflict & DMG_RADIATION )
	{
		SetThink( &CTriggerHurt::RadiationThink );
		pev->nextthink = gpGlobals->time + RANDOM_FLOAT( 0.0, 0.5 );
	}

	if( FBitSet( pev->spawnflags, SF_TRIGGER_HURT_START_OFF ) )// if flagged to Start Turned Off, make trigger nonsolid.
		pev->solid = SOLID_NOT;

	SetAbsOrigin( GetAbsOrigin() );		// Link into the list
}

// trigger hurt that causes radiation will do a radius
// check and set the player's geiger counter level
// according to distance from center of trigger

void CTriggerHurt::RadiationThink( void )
{
	CBasePlayer *pPlayer = NULL;
	float flRange;
	Vector vecSpot1;
	Vector vecSpot2;
	Vector vecRange;
	Vector origin;
	Vector view_ofs;

	// check to see if a player is in pvs
	// if not, continue	

	// set origin to center of trigger so that this check works
	origin = GetAbsOrigin();
	view_ofs = pev->view_ofs;

	pev->origin = ( pev->absmin + pev->absmax ) * 0.5;
	pev->view_ofs = pev->view_ofs * 0.0;

	CBaseEntity* pentPlayer = UTIL_FindClientInPVS( this );

	pev->origin = origin;
	pev->view_ofs = view_ofs;

	// reset origin

	if( !FNullEnt( pentPlayer ) )
	{
		pPlayer = static_cast<CBasePlayer*>( pentPlayer );

		// get range to player;

		vecSpot1 = ( pev->absmin + pev->absmax ) * 0.5;
		vecSpot2 = ( pPlayer->pev->absmin + pPlayer->pev->absmax ) * 0.5;

		vecRange = vecSpot1 - vecSpot2;
		flRange = vecRange.Length();

		// if player's current geiger counter range is larger
		// than range to this trigger hurt, reset player's
		// geiger counter range 

		if( pPlayer->m_flgeigerRange >= flRange )
			pPlayer->m_flgeigerRange = flRange;
	}

	pev->nextthink = gpGlobals->time + 0.25;
}

// When touched, a hurt trigger does DMG points of damage each half-second
void CTriggerHurt::HurtTouch( CBaseEntity *pOther )
{
	float fldmg;

	if( !pOther->pev->takedamage )
		return;

	if( ( pev->spawnflags & SF_TRIGGER_HURT_CLIENTONLYTOUCH ) && !pOther->IsPlayer() )
	{
		// this trigger is only allowed to touch clients, and this ain't a client.
		return;
	}

	if( ( pev->spawnflags & SF_TRIGGER_HURT_NO_CLIENTS ) && pOther->IsPlayer() )
		return;

	// HACKHACK -- In multiplayer, players touch this based on packet receipt.
	// So the players who send packets later aren't always hurt.  Keep track of
	// how much time has passed and whether or not you've touched that player
	if( g_pGameRules->IsMultiplayer() )
	{
		if( pev->dmgtime > gpGlobals->time )
		{
			if( gpGlobals->time != pev->pain_finished )
			{// too early to hurt again, and not same frame with a different entity
				if( pOther->IsPlayer() )
				{
					int playerMask = 1 << ( pOther->entindex() - 1 );

					// If I've already touched this player (this time), then bail out
					if( pev->impulse & playerMask )
						return;

					// Mark this player as touched
					// BUGBUG - There can be only 32 players!
					pev->impulse |= playerMask;
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			// New clock, "un-touch" all players
			pev->impulse = 0;
			if( pOther->IsPlayer() )
			{
				int playerMask = 1 << ( pOther->entindex() - 1 );

				// Mark this player as touched
				// BUGBUG - There can be only 32 players!
				pev->impulse |= playerMask;
			}
		}
	}
	else	// Original code -- single player
	{
		if( pev->dmgtime > gpGlobals->time && gpGlobals->time != pev->pain_finished )
		{// too early to hurt again, and not same frame with a different entity
			return;
		}
	}



	// If this is time_based damage (poison, radiation), override the pev->dmg with a 
	// default for the given damage type.  Monsters only take time-based damage
	// while touching the trigger.  Player continues taking damage for a while after
	// leaving the trigger

	fldmg = pev->dmg * 0.5;	// 0.5 seconds worth of damage, pev->dmg is damage/second


							// JAY: Cut this because it wasn't fully realized.  Damage is simpler now.
#if 0
	switch( m_bitsDamageInflict )
	{
	default: break;
	case DMG_POISON:		fldmg = POISON_DAMAGE / 4; break;
	case DMG_NERVEGAS:		fldmg = NERVEGAS_DAMAGE / 4; break;
	case DMG_RADIATION:		fldmg = RADIATION_DAMAGE / 4; break;
	case DMG_PARALYZE:		fldmg = PARALYZE_DAMAGE / 4; break; // UNDONE: cut this? should slow movement to 50%
	case DMG_ACID:			fldmg = ACID_DAMAGE / 4; break;
	case DMG_SLOWBURN:		fldmg = SLOWBURN_DAMAGE / 4; break;
	case DMG_SLOWFREEZE:	fldmg = SLOWFREEZE_DAMAGE / 4; break;
	}
#endif

	if( fldmg < 0 )
		pOther->GiveHealth( -fldmg, m_bitsDamageInflict );
	else
		pOther->TakeDamage( this, this, fldmg, m_bitsDamageInflict );

	// Store pain time so we can get all of the other entities on this frame
	pev->pain_finished = gpGlobals->time;

	// Apply damage every half second
	pev->dmgtime = gpGlobals->time + 0.5;// half second delay until this trigger can hurt toucher again



	if( HasTarget() )
	{
		// trigger has a target it wants to fire. 
		if( pev->spawnflags & SF_TRIGGER_HURT_CLIENTONLYFIRE )
		{
			// if the toucher isn't a client, don't fire the target!
			if( !pOther->IsPlayer() )
			{
				return;
			}
		}

		SUB_UseTargets( pOther, USE_TOGGLE, 0 );
		if( pev->spawnflags & SF_TRIGGER_HURT_TARGETONCE )
			pev->target = 0;
	}
}