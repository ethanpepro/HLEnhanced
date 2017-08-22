/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#include "extdll.h"
#include "util.h"
#include "Skill.h"
#include "cbase.h"
#include "Weapons.h"

#include "CSentry.h"

BEGIN_DATADESC( CSentry )
	DEFINE_TOUCHFUNC( SentryTouch ),
	DEFINE_THINKFUNC( SentryDeath ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( monster_sentry, CSentry );

void CSentry::Spawn()
{
	Precache();
	SetModel( "models/sentry.mdl" );
	pev->health = gSkillData.GetSentryHealth();
	m_HackedGunPos = Vector( 0, 0, 48 );
	pev->view_ofs.z = 48;
	m_flMaxWait = 1E6;
	m_flMaxSpin = 1E6;

	CBaseTurret::Spawn();
	m_iRetractHeight = 64;
	m_iDeployHeight = 64;
	m_iMinPitch = -60;
	SetSize( Vector( -16, -16, -m_iRetractHeight ), Vector( 16, 16, m_iRetractHeight ) );

	SetTouch( &CSentry::SentryTouch );
	SetThink( &CSentry::Initialize );
	pev->nextthink = gpGlobals->time + 0.3;
}

void CSentry::Precache()
{
	CBaseTurret::Precache();
	PRECACHE_MODEL( "models/sentry.mdl" );
}

void CSentry::Shoot( Vector &vecSrc, Vector &vecDirToEnemy )
{
	FireBullets( 1, vecSrc, vecDirToEnemy, TURRET_SPREAD, TURRET_RANGE, BULLET_MONSTER_MP5, 1 );

	switch( RANDOM_LONG( 0, 2 ) )
	{
	case 0: EMIT_SOUND( this, CHAN_WEAPON, "weapons/hks1.wav", 1, ATTN_NORM ); break;
	case 1: EMIT_SOUND( this, CHAN_WEAPON, "weapons/hks2.wav", 1, ATTN_NORM ); break;
	case 2: EMIT_SOUND( this, CHAN_WEAPON, "weapons/hks3.wav", 1, ATTN_NORM ); break;
	}
	pev->effects = pev->effects | EF_MUZZLEFLASH;
}

void CSentry::OnTakeDamage( const CTakeDamageInfo& info )
{
	if( !pev->takedamage )
		return;

	if( !m_bOn )
	{
		SetThink( &CSentry::Deploy );
		SetUse( NULL );
		pev->nextthink = gpGlobals->time + 0.1;
	}

	pev->health -= info.GetDamage();
	if( pev->health <= 0 )
	{
		pev->health = 0;
		pev->takedamage = DAMAGE_NO;
		pev->dmgtime = gpGlobals->time;

		ClearBits( pev->flags, FL_MONSTER ); // why are they set in the first place???

		SetUse( NULL );
		SetThink( &CSentry::SentryDeath );
		SUB_UseTargets( this, USE_ON, 0 ); // wake up others
		pev->nextthink = gpGlobals->time + 0.1;
	}
}

void CSentry::SentryTouch( CBaseEntity *pOther )
{
	if( pOther && ( pOther->IsPlayer() || ( pOther->pev->flags & FL_MONSTER ) ) )
	{
		TakeDamage( pOther, pOther, 0, 0 );
	}
}

void CSentry::SentryDeath( void )
{
	StudioFrameAdvance();
	pev->nextthink = gpGlobals->time + 0.1;

	if( pev->deadflag != DEAD_DEAD )
	{
		pev->deadflag = DEAD_DEAD;

		float flRndSound = RANDOM_FLOAT( 0, 1 );

		if( flRndSound <= 0.33 )
			EMIT_SOUND( this, CHAN_BODY, "turret/tu_die.wav", 1.0, ATTN_NORM );
		else if( flRndSound <= 0.66 )
			EMIT_SOUND( this, CHAN_BODY, "turret/tu_die2.wav", 1.0, ATTN_NORM );
		else
			EMIT_SOUND( this, CHAN_BODY, "turret/tu_die3.wav", 1.0, ATTN_NORM );

		EMIT_SOUND_DYN( this, CHAN_STATIC, "turret/tu_active2.wav", 0, 0, SND_STOP, 100 );

		SetBoneController( 0, 0 );
		SetBoneController( 1, 0 );

		SetTurretAnim( TURRET_ANIM_DIE );

		pev->solid = SOLID_NOT;
		pev->angles.y = UTIL_AngleMod( pev->angles.y + RANDOM_LONG( 0, 2 ) * 120 );

		EyeOn();
	}

	EyeOff();

	Vector vecSrc, vecAng;
	GetAttachment( 1, vecSrc, vecAng );

	if( pev->dmgtime + RANDOM_FLOAT( 0, 2 ) > gpGlobals->time )
	{
		// lots of smoke
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_SMOKE );
		WRITE_COORD( vecSrc.x + RANDOM_FLOAT( -16, 16 ) );
		WRITE_COORD( vecSrc.y + RANDOM_FLOAT( -16, 16 ) );
		WRITE_COORD( vecSrc.z - 32 );
		WRITE_SHORT( g_sModelIndexSmoke );
		WRITE_BYTE( 15 ); // scale * 10
		WRITE_BYTE( 8 ); // framerate
		MESSAGE_END();
	}

	if( pev->dmgtime + RANDOM_FLOAT( 0, 8 ) > gpGlobals->time )
	{
		UTIL_Sparks( vecSrc );
	}

	if( m_fSequenceFinished && pev->dmgtime + 5 < gpGlobals->time )
	{
		pev->framerate = 0;
		SetThink( NULL );
	}
}