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
#include "entities/DoorConstants.h"

#include "CPendulum.h"

BEGIN_DATADESC( CPendulum )
	DEFINE_FIELD( m_accel, FIELD_FLOAT ),
	DEFINE_FIELD( m_distance, FIELD_FLOAT ),
	DEFINE_FIELD( m_time, FIELD_TIME ),
	DEFINE_FIELD( m_damp, FIELD_FLOAT ),
	DEFINE_FIELD( m_maxSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( m_dampSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( m_center, FIELD_VECTOR ),
	DEFINE_FIELD( m_start, FIELD_VECTOR ),
	DEFINE_THINKFUNC( Swing ),
	DEFINE_USEFUNC( PendulumUse ),
	DEFINE_THINKFUNC( Stop ),
	DEFINE_TOUCHFUNC( RopeTouch ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( func_pendulum, CPendulum );

void CPendulum::Spawn( void )
{
	// set the axis of rotation
	CBaseToggle::AxisDir( this );

	if( FBitSet( pev->spawnflags, SF_DOOR_PASSABLE ) )
		pev->solid = SOLID_NOT;
	else
		pev->solid = SOLID_BSP;
	pev->movetype = MOVETYPE_PUSH;
	SetAbsOrigin( GetAbsOrigin() );
	SetModel( STRING( pev->model ) );

	if( m_distance == 0 )
		return;

	if( pev->speed == 0 )
		pev->speed = 100;

	m_accel = ( pev->speed * pev->speed ) / ( 2 * fabs( m_distance ) );	// Calculate constant acceleration from speed and distance
	m_maxSpeed = pev->speed;
	m_start = pev->angles;
	m_center = pev->angles + ( m_distance * 0.5 ) * pev->movedir;

	if( FBitSet( pev->spawnflags, SF_BRUSH_ROTATE_INSTANT ) )
	{
		SetThink( &CPendulum::SUB_CallUseToggle );
		pev->nextthink = gpGlobals->time + 0.1;
	}
	pev->speed = 0;
	SetUse( &CPendulum::PendulumUse );

	if( FBitSet( pev->spawnflags, SF_PENDULUM_SWING ) )
	{
		SetTouch( &CPendulum::RopeTouch );
	}
}

void CPendulum::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "distance" ) )
	{
		m_distance = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "damp" ) )
	{
		m_damp = atof( pkvd->szValue ) * 0.001;
		pkvd->fHandled = true;
	}
	else
		CBaseEntity::KeyValue( pkvd );
}

void CPendulum::Swing( void )
{
	float delta, dt;

	delta = CBaseToggle::AxisDelta( pev->spawnflags, pev->angles, m_center );
	dt = gpGlobals->time - m_time;	// How much time has passed?
	m_time = gpGlobals->time;		// Remember the last time called

	if( delta > 0 && m_accel > 0 )
		pev->speed -= m_accel * dt;	// Integrate velocity
	else
		pev->speed += m_accel * dt;

	if( pev->speed > m_maxSpeed )
		pev->speed = m_maxSpeed;
	else if( pev->speed < -m_maxSpeed )
		pev->speed = -m_maxSpeed;
	// scale the destdelta vector by the time spent traveling to get velocity
	pev->avelocity = pev->speed * pev->movedir;

	// Call this again
	pev->nextthink = pev->ltime + 0.1;

	if( m_damp )
	{
		m_dampSpeed -= m_damp * m_dampSpeed * dt;
		if( m_dampSpeed < 30.0 )
		{
			pev->angles = m_center;
			pev->speed = 0;
			SetThink( NULL );
			pev->avelocity = g_vecZero;
		}
		else if( pev->speed > m_dampSpeed )
			pev->speed = m_dampSpeed;
		else if( pev->speed < -m_dampSpeed )
			pev->speed = -m_dampSpeed;

	}
}

void CPendulum::PendulumUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( pev->speed )		// Pendulum is moving, stop it and auto-return if necessary
	{
		if( FBitSet( pev->spawnflags, SF_PENDULUM_AUTO_RETURN ) )
		{
			float	delta;

			delta = CBaseToggle::AxisDelta( pev->spawnflags, pev->angles, m_start );

			pev->avelocity = m_maxSpeed * pev->movedir;
			pev->nextthink = pev->ltime + ( delta / m_maxSpeed );
			SetThink( &CPendulum::Stop );
		}
		else
		{
			pev->speed = 0;		// Dead stop
			SetThink( NULL );
			pev->avelocity = g_vecZero;
		}
	}
	else
	{
		pev->nextthink = pev->ltime + 0.1;		// Start the pendulum moving
		m_time = gpGlobals->time;		// Save time to calculate dt
		SetThink( &CPendulum::Swing );
		m_dampSpeed = m_maxSpeed;
	}
}

void CPendulum::Stop( void )
{
	pev->angles = m_start;
	pev->speed = 0;
	SetThink( NULL );
	pev->avelocity = g_vecZero;
}

void CPendulum::Touch( CBaseEntity *pOther )
{
	if( pev->dmg <= 0 )
		return;

	// we can't hurt this thing, so we're not concerned with it
	if( !pOther->pev->takedamage )
		return;

	// calculate damage based on rotation speed
	float damage = pev->dmg * pev->speed * 0.01;

	if( damage < 0 )
		damage = -damage;

	pOther->TakeDamage( this, this, damage, DMG_CRUSH );

	pOther->pev->velocity = ( pOther->GetAbsOrigin() - VecBModelOrigin( this ) ).Normalize() * damage;
}

void CPendulum::RopeTouch( CBaseEntity *pOther )
{
	if( !pOther->IsPlayer() )
	{// not a player!
		ALERT( at_console, "Not a client\n" );
		return;
	}

	if( ENT( pOther->pev ) == pev->enemy )
	{// this player already on the rope.
		return;
	}

	pev->enemy = pOther->edict();
	pOther->pev->velocity = g_vecZero;
	pOther->pev->movetype = MOVETYPE_NONE;
}

void CPendulum::Blocked( CBaseEntity *pOther )
{
	m_time = gpGlobals->time;
}