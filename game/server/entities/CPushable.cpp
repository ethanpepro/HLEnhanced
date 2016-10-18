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

#include "CPushable.h"

const char* const CPushable::m_soundNames[ 3 ] = { "debris/pushbox1.wav", "debris/pushbox2.wav", "debris/pushbox3.wav" };

BEGIN_DATADESC( CPushable )
	DEFINE_FIELD( m_maxSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( m_soundTime, FIELD_TIME ),
#if 0
	DEFINE_THINKFUNC( StopSound ),
#endif
END_DATADESC()

LINK_ENTITY_TO_CLASS( func_pushable, CPushable );

void CPushable::Spawn( void )
{
	if( pev->spawnflags & SF_PUSH_BREAKABLE )
		CBreakable::Spawn();
	else
		Precache();

	pev->movetype = MOVETYPE_PUSHSTEP;
	pev->solid = SOLID_BBOX;
	SetModel( STRING( pev->model ) );

	if( pev->friction > 399 )
		pev->friction = 399;

	m_maxSpeed = 400 - pev->friction;
	GetFlags() |= FL_FLOAT;
	pev->friction = 0;

	pev->origin.z += 1;	// Pick up off of the floor
	SetAbsOrigin( GetAbsOrigin() );

	// Multiply by area of the box's cross-section (assume 1000 units^3 standard volume)
	pev->skin = ( pev->skin * ( pev->maxs.x - pev->mins.x ) * ( pev->maxs.y - pev->mins.y ) ) * 0.0005;
	m_soundTime = 0;
}

void CPushable::Precache( void )
{
	for( int i = 0; i < 3; i++ )
		PRECACHE_SOUND( m_soundNames[ i ] );

	if( pev->spawnflags & SF_PUSH_BREAKABLE )
		CBreakable::Precache();
}

void CPushable::Touch( CBaseEntity *pOther )
{
	if( pOther->ClassnameIs( "worldspawn" ) )
		return;

	Move( pOther, 1 );
}

void CPushable::Move( CBaseEntity *pOther, int push )
{
	int playerTouch = 0;

	// Is entity standing on this pushable ?
	if( pOther->GetFlags().Any( FL_ONGROUND ) && pOther->pev->groundentity && GET_PRIVATE( pOther->pev->groundentity ) == this )
	{
		// Only push if floating
		if( GetWaterLevel() > WATERLEVEL_DRY )
			pev->velocity.z += pOther->pev->velocity.z * 0.1;

		return;
	}


	if( pOther->IsPlayer() )
	{
		if( push && !( pOther->pev->button & ( IN_FORWARD | IN_USE ) ) )	// Don't push unless the player is pushing forward and NOT use (pull)
			return;
		playerTouch = 1;
	}

	float factor;

	if( playerTouch )
	{
		if( !pOther->GetFlags().Any( FL_ONGROUND ) )	// Don't push away from jumping/falling players unless in water
		{
			if( GetWaterLevel() < WATERLEVEL_FEET )
				return;
			else
				factor = 0.1;
		}
		else
			factor = 1;
	}
	else
		factor = 0.25;

	pev->velocity.x += pOther->pev->velocity.x * factor;
	pev->velocity.y += pOther->pev->velocity.y * factor;

	float length = sqrt( pev->velocity.x * pev->velocity.x + pev->velocity.y * pev->velocity.y );
	if( push && ( length > MaxSpeed() ) )
	{
		pev->velocity.x = ( pev->velocity.x * MaxSpeed() / length );
		pev->velocity.y = ( pev->velocity.y * MaxSpeed() / length );
	}
	if( playerTouch )
	{
		pOther->pev->velocity.x = pev->velocity.x;
		pOther->pev->velocity.y = pev->velocity.y;
		if( ( gpGlobals->time - m_soundTime ) > 0.7 )
		{
			m_soundTime = gpGlobals->time;
			if( length > 0 && GetFlags().Any( FL_ONGROUND ) )
			{
				m_lastSound = RANDOM_LONG( 0, 2 );
				EMIT_SOUND( this, CHAN_WEAPON, m_soundNames[ m_lastSound ], 0.5, ATTN_NORM );
				//			SetThink( StopSound );
				//			pev->nextthink = pev->ltime + 0.1;
			}
			else
				STOP_SOUND( this, CHAN_WEAPON, m_soundNames[ m_lastSound ] );
		}
	}
}

void CPushable::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "size" ) )
	{
		int bbox = atoi( pkvd->szValue );
		pkvd->fHandled = true;

		switch( bbox )
		{
		case 0:	// Point
			SetSize( Vector( -8, -8, -8 ), Vector( 8, 8, 8 ) );
			break;

		case 2: // Big Hull!?!?	!!!BUGBUG Figure out what this hull really is
			SetSize( VEC_DUCK_HULL_MIN * 2, VEC_DUCK_HULL_MAX * 2 );
			break;

		case 3: // Player duck
			SetSize( VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX );
			break;

		default:
		case 1: // Player
			SetSize( VEC_HULL_MIN, VEC_HULL_MAX );
			break;
		}

	}
	else if( FStrEq( pkvd->szKeyName, "buoyancy" ) )
	{
		pev->skin = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBreakable::KeyValue( pkvd );
}

// Pull the func_pushable
void CPushable::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( !pActivator || !pActivator->IsPlayer() )
	{
		if( pev->spawnflags & SF_PUSH_BREAKABLE )
			this->CBreakable::Use( pActivator, pCaller, useType, value );
		return;
	}

	if( pActivator->pev->velocity != g_vecZero )
		Move( pActivator, 0 );
}

#if 0
void CPushable::StopSound( void )
{
	Vector dist = GetOldOrigin() - GetAbsOrigin();
	if( dist.Length() <= 0 )
		STOP_SOUND( this, CHAN_WEAPON, m_soundNames[ m_lastSound ] );
}
#endif

void CPushable::OnTakeDamage( const CTakeDamageInfo& info )
{
	if( pev->spawnflags & SF_PUSH_BREAKABLE )
		CBreakable::OnTakeDamage( info );
}
