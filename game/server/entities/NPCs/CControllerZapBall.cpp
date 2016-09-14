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

#include "CControllerZapBall.h"

BEGIN_DATADESC( CControllerZapBall )
	DEFINE_THINKFUNC( AnimateThink ),
	DEFINE_TOUCHFUNC( ExplodeTouch ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( controller_energy_ball, CControllerZapBall );

void CControllerZapBall::Spawn( void )
{
	Precache();
	// motor
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	SetModel( "sprites/xspark4.spr" );
	pev->rendermode = kRenderTransAdd;
	pev->rendercolor.x = 255;
	pev->rendercolor.y = 255;
	pev->rendercolor.z = 255;
	pev->renderamt = 255;
	pev->scale = 0.5;

	SetSize( Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );
	SetAbsOrigin( GetAbsOrigin() );

	SetThink( &CControllerZapBall::AnimateThink );
	SetTouch( &CControllerZapBall::ExplodeTouch );

	m_hOwner = Instance( pev->owner );
	pev->dmgtime = gpGlobals->time; // keep track of when ball spawned
	pev->nextthink = gpGlobals->time + 0.1;
}

void CControllerZapBall::Precache( void )
{
	PRECACHE_MODEL( "sprites/xspark4.spr" );
	// PRECACHE_SOUND("debris/zap4.wav");
	// PRECACHE_SOUND("weapons/electro4.wav");
}

void CControllerZapBall::AnimateThink( void )
{
	pev->nextthink = gpGlobals->time + 0.1;

	pev->frame = ( ( int ) pev->frame + 1 ) % 11;

	if( gpGlobals->time - pev->dmgtime > 5 || pev->velocity.Length() < 10 )
	{
		SetTouch( NULL );
		UTIL_Remove( this );
	}
}

void CControllerZapBall::ExplodeTouch( CBaseEntity *pOther )
{
	if( pOther->pev->takedamage )
	{
		TraceResult tr = UTIL_GetGlobalTrace();

		CBaseEntity* pOwner;
		if( m_hOwner != nullptr )
		{
			pOwner = m_hOwner;
		}
		else
		{
			pOwner = this;
		}

		g_MultiDamage.Clear();
		pOther->TraceAttack( CTakeDamageInfo( pOwner, gSkillData.GetControllerDmgBall(), DMG_ENERGYBEAM ), pev->velocity.Normalize(), &tr );
		g_MultiDamage.ApplyMultiDamage( pOwner, pOwner );

		UTIL_EmitAmbientSound( this, tr.vecEndPos, "weapons/electro4.wav", 0.3, ATTN_NORM, 0, RANDOM_LONG( 90, 99 ) );

	}

	UTIL_Remove( this );
}