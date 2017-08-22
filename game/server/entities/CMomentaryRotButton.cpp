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

#include "CMomentaryRotButton.h"

BEGIN_DATADESC( CMomentaryRotButton )
	DEFINE_FIELD( m_lastUsed, FIELD_INTEGER ),
	DEFINE_FIELD( m_direction, FIELD_INTEGER ),
	DEFINE_FIELD( m_returnSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( m_start, FIELD_VECTOR ),
	DEFINE_FIELD( m_end, FIELD_VECTOR ),
	DEFINE_FIELD( m_sounds, FIELD_INTEGER ),

	DEFINE_THINKFUNC( Off ),
	DEFINE_THINKFUNC( Return ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( momentary_rot_button, CMomentaryRotButton );

void CMomentaryRotButton::Spawn( void )
{
	CBaseToggle::AxisDir( this );

	if( pev->speed == 0 )
		pev->speed = 100;

	if( m_flMoveDistance < 0 )
	{
		m_start = pev->angles + pev->movedir * m_flMoveDistance;
		m_end = pev->angles;
		m_direction = 1;		// This will toggle to -1 on the first use()
		m_flMoveDistance = -m_flMoveDistance;
	}
	else
	{
		m_start = pev->angles;
		m_end = pev->angles + pev->movedir * m_flMoveDistance;
		m_direction = -1;		// This will toggle to +1 on the first use()
	}

	if( pev->spawnflags & SF_MOMENTARY_DOOR )
		pev->solid = SOLID_BSP;
	else
		pev->solid = SOLID_NOT;

	pev->movetype = MOVETYPE_PUSH;
	SetAbsOrigin( GetAbsOrigin() );
	SetModel( STRING( pev->model ) );

	const char* pszSound = ButtonSound( m_sounds );
	PRECACHE_SOUND( pszSound );
	pev->noise = ALLOC_STRING( pszSound );
	m_lastUsed = 0;
}

void CMomentaryRotButton::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "returnspeed" ) )
	{
		m_returnSpeed = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "sounds" ) )
	{
		m_sounds = atoi( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseToggle::KeyValue( pkvd );
}

// BUGBUG: This design causes a latentcy.  When the button is retriggered, the first impulse
// will send the target in the wrong direction because the parameter is calculated based on the
// current, not future position.
void CMomentaryRotButton::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	pev->ideal_yaw = CBaseToggle::AxisDelta( pev->spawnflags, pev->angles, m_start ) / m_flMoveDistance;

	UpdateAllButtons( pev->ideal_yaw, 1 );

	// Calculate destination angle and use it to predict value, this prevents sending target in wrong direction on retriggering
	Vector dest = pev->angles + pev->avelocity * ( pev->nextthink - pev->ltime );
	float value1 = CBaseToggle::AxisDelta( pev->spawnflags, dest, m_start ) / m_flMoveDistance;
	UpdateTarget( value1 );

}

void CMomentaryRotButton::Off( void )
{
	pev->avelocity = g_vecZero;
	m_lastUsed = 0;
	if( FBitSet( pev->spawnflags, SF_PENDULUM_AUTO_RETURN ) && m_returnSpeed > 0 )
	{
		SetThink( &CMomentaryRotButton::Return );
		pev->nextthink = pev->ltime + 0.1;
		m_direction = -1;
	}
	else
		SetThink( NULL );
}

void CMomentaryRotButton::Return( void )
{
	float value = CBaseToggle::AxisDelta( pev->spawnflags, pev->angles, m_start ) / m_flMoveDistance;

	UpdateAllButtons( value, 0 );	// This will end up calling UpdateSelfReturn() n times, but it still works right
	if( value > 0 )
		UpdateTarget( value );
}

void CMomentaryRotButton::UpdateSelf( float value )
{
	bool fplaysound = false;

	if( !m_lastUsed )
	{
		fplaysound = true;
		m_direction = -m_direction;
	}
	m_lastUsed = 1;

	pev->nextthink = pev->ltime + 0.1;
	if( m_direction > 0 && value >= 1.0 )
	{
		pev->avelocity = g_vecZero;
		pev->angles = m_end;
		return;
	}
	else if( m_direction < 0 && value <= 0 )
	{
		pev->avelocity = g_vecZero;
		pev->angles = m_start;
		return;
	}

	if( fplaysound )
		PlaySound();

	// HACKHACK -- If we're going slow, we'll get multiple player packets per frame, bump nexthink on each one to avoid stalling
	if( pev->nextthink < pev->ltime )
		pev->nextthink = pev->ltime + 0.1;
	else
		pev->nextthink += 0.1;

	pev->avelocity = ( m_direction * pev->speed ) * pev->movedir;
	SetThink( &CMomentaryRotButton::Off );
}

void CMomentaryRotButton::UpdateSelfReturn( float value )
{
	if( value <= 0 )
	{
		pev->avelocity = g_vecZero;
		pev->angles = m_start;
		pev->nextthink = -1;
		SetThink( NULL );
	}
	else
	{
		pev->avelocity = -m_returnSpeed * pev->movedir;
		pev->nextthink = pev->ltime + 0.1;
	}
}

void CMomentaryRotButton::UpdateAllButtons( float value, int start )
{
	// Update all rot buttons attached to the same target
	CBaseEntity* pTarget = nullptr;
	while( ( pTarget = UTIL_FindEntityByTarget( pTarget, GetTarget() ) ) != nullptr )
	{
		if( pTarget->ClassnameIs( "momentary_rot_button" ) )
		{
			if( CMomentaryRotButton* pEntity = CMomentaryRotButton::Instance( pTarget ) )
			{
				if( start )
					pEntity->UpdateSelf( value );
				else
					pEntity->UpdateSelfReturn( value );
			}
		}
	}
}

void CMomentaryRotButton::PlaySound( void )
{
	EMIT_SOUND( this, CHAN_VOICE, ( char* ) STRING( pev->noise ), 1, ATTN_NORM );
}

void CMomentaryRotButton::UpdateTarget( float value )
{
	if( HasTarget() )
	{
		CBaseEntity* pTarget = nullptr;

		while( ( pTarget = UTIL_FindEntityByTargetname( pTarget, GetTarget() ) ) != nullptr )
		{
			pTarget->Use( this, this, USE_SET, value );
		}
	}
}
