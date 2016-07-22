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
#include "cbase.h"
#include "Weapons.h"

#include "CSmoker.h"

LINK_ENTITY_TO_CLASS( env_smoker, CSmoker );

void CSmoker::Spawn( void )
{
	pev->movetype = MOVETYPE_NONE;
	pev->nextthink = gpGlobals->time;
	pev->solid = SOLID_NOT;
	SetSize( g_vecZero, g_vecZero );
	pev->effects |= EF_NODRAW;
	pev->angles = g_vecZero;
}

void CSmoker::Think( void )
{
	// lots of smoke
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, GetAbsOrigin() );
	WRITE_BYTE( TE_SMOKE );
	WRITE_COORD( GetAbsOrigin().x + RANDOM_FLOAT( -pev->dmg, pev->dmg ) );
	WRITE_COORD( GetAbsOrigin().y + RANDOM_FLOAT( -pev->dmg, pev->dmg ) );
	WRITE_COORD( GetAbsOrigin().z );
	WRITE_SHORT( g_sModelIndexSmoke );
	WRITE_BYTE( RANDOM_LONG( pev->scale, pev->scale * 1.1 ) );
	WRITE_BYTE( RANDOM_LONG( 8, 14 ) ); // framerate
	MESSAGE_END();

	pev->health--;
	if( pev->health > 0 )
		pev->nextthink = gpGlobals->time + RANDOM_FLOAT( 0.1, 0.2 );
	else
		UTIL_Remove( this );
}