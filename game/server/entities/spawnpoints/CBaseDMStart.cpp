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

#include "CBaseDMStart.h"

// These are the new entry points to entities. 
//TODO: use a common base class for all spawn points for finer control - Solokiller
LINK_ENTITY_TO_CLASS( info_player_deathmatch, CBaseDMStart );
LINK_ENTITY_TO_CLASS( info_player_start, CPointEntity );
LINK_ENTITY_TO_CLASS( info_landmark, CPointEntity );

void CBaseDMStart::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "master" ) )
	{
		pev->netname = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CPointEntity::KeyValue( pkvd );
}

bool CBaseDMStart::IsTriggered( const CBaseEntity* const pEntity ) const
{
	bool master = UTIL_IsMasterTriggered( pev->netname, pEntity );

	return master;
}