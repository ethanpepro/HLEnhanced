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

#include "CFuncWallToggle.h"

LINK_ENTITY_TO_CLASS( func_wall_toggle, CFuncWallToggle );

void CFuncWallToggle::Spawn( void )
{
	CFuncWall::Spawn();
	if( pev->spawnflags & SF_WALL_START_OFF )
		TurnOff();
}

void CFuncWallToggle::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	const bool status = IsOn();

	if( ShouldToggle( useType, status ) )
	{
		if( status )
			TurnOff();
		else
			TurnOn();
	}
}

void CFuncWallToggle::TurnOff( void )
{
	pev->solid = SOLID_NOT;
	pev->effects |= EF_NODRAW;
	SetAbsOrigin( GetAbsOrigin() );
}

void CFuncWallToggle::TurnOn( void )
{
	pev->solid = SOLID_BSP;
	pev->effects &= ~EF_NODRAW;
	SetAbsOrigin( GetAbsOrigin() );
}

bool CFuncWallToggle::IsOn() const
{
	if( pev->solid == SOLID_NOT )
		return false;
	return true;
}