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
#include "Decals.h"
#include "cbase.h"
#include "Weapons.h"

#include "TempEntity.h"

#include "CBMortar.h"

extern int gSpitSprite;

BEGIN_DATADESC( CBMortar )
	DEFINE_FIELD( m_maxFrame, FIELD_INTEGER ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( bmortar, CBMortar );

// UNDONE: right now this is pretty much a copy of the squid spit with minor changes to the way it does damage
void CBMortar::Spawn( void )
{
	pev->movetype = MOVETYPE_TOSS;
	pev->classname = MAKE_STRING( "bmortar" );

	pev->solid = SOLID_BBOX;
	pev->rendermode = kRenderTransAlpha;
	pev->renderamt = 255;

	SET_MODEL( ENT( pev ), "sprites/mommaspit.spr" );
	pev->frame = 0;
	pev->scale = 0.5;

	UTIL_SetSize( this, Vector( 0, 0, 0 ), Vector( 0, 0, 0 ) );

	m_maxFrame = ( float ) MODEL_FRAMES( pev->modelindex ) - 1;
	pev->dmgtime = gpGlobals->time + 0.4;
}

CBMortar *CBMortar::Shoot( CBaseEntity* pOwner, Vector vecStart, Vector vecVelocity )
{
	CBMortar *pSpit = GetClassPtr( ( CBMortar * ) NULL );
	pSpit->Spawn();

	UTIL_SetOrigin( pSpit, vecStart );
	pSpit->pev->velocity = vecVelocity;
	pSpit->pev->owner = pOwner->edict();
	pSpit->pev->scale = 2.5;
	pSpit->SetThink( &CBMortar::Animate );
	pSpit->pev->nextthink = gpGlobals->time + 0.1;

	return pSpit;
}

void CBMortar::Touch( CBaseEntity *pOther )
{
	TraceResult tr;
	int		iPitch;

	// splat sound
	iPitch = RANDOM_FLOAT( 90, 110 );

	EMIT_SOUND_DYN( ENT( pev ), CHAN_VOICE, "bullchicken/bc_acid1.wav", 1, ATTN_NORM, 0, iPitch );

	switch( RANDOM_LONG( 0, 1 ) )
	{
	case 0:
		EMIT_SOUND_DYN( ENT( pev ), CHAN_WEAPON, "bullchicken/bc_spithit1.wav", 1, ATTN_NORM, 0, iPitch );
		break;
	case 1:
		EMIT_SOUND_DYN( ENT( pev ), CHAN_WEAPON, "bullchicken/bc_spithit2.wav", 1, ATTN_NORM, 0, iPitch );
		break;
	}

	if( pOther->IsBSPModel() )
	{

		// make a splat on the wall
		UTIL_TraceLine( pev->origin, pev->origin + pev->velocity * 10, dont_ignore_monsters, ENT( pev ), &tr );
		UTIL_DecalTrace( &tr, DECAL_MOMMASPLAT );
	}
	else
	{
		tr.vecEndPos = pev->origin;
		tr.vecPlaneNormal = -1 * pev->velocity.Normalize();
	}
	// make some flecks
	SpriteSpray( tr.vecEndPos, tr.vecPlaneNormal, gSpitSprite, 24 );

	CBaseEntity* pOwner = pev->owner ? Instance( pev->owner ) : nullptr;

	RadiusDamage( pev->origin, CTakeDamageInfo( this, pOwner, gSkillData.bigmommaDmgBlast, DMG_ACID ), gSkillData.bigmommaRadiusBlast, CLASS_NONE );
	UTIL_Remove( this );
}

void CBMortar::Animate( void )
{
	pev->nextthink = gpGlobals->time + 0.1;

	if( gpGlobals->time > pev->dmgtime )
	{
		pev->dmgtime = gpGlobals->time + 0.2;
		SpriteSpray( pev->origin, -pev->velocity.Normalize(), gSpitSprite, 3 );
	}
	if( pev->frame++ )
	{
		if( pev->frame > m_maxFrame )
		{
			pev->frame = 0;
		}
	}
}