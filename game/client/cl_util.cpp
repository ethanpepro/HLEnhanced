/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
//
// cl_util.cpp
//
// implementation of class-less helper functions
//

#include "hud.h"
#include "cl_util.h"
#include <string.h>

HSPRITE LoadSprite( const char* const pszName )
{
	char sz[ MAX_PATH ];

	const int iResolution = ScreenWidth < 640 ? 320 : 640;

	const int iResult = snprintf( sz, sizeof( sz ), pszName, iResolution );

	//Handle formatting failure. - Solokiller
	if( iResult < 0 || static_cast<size_t>( iResult ) >= sizeof( sz ) )
	{
		gEngfuncs.Con_Printf( "LoadSprite: Failed to load HUD sprite \"%s\" for resolution %d\n", pszName, iResolution );
		return INVALID_HSPRITE;
	}

	return SPR_Load( sz );
}

/* =================================
GetSpriteList

Finds and returns the matching
sprite name 'psz' and resolution 'iRes'
in the given sprite list 'pList'
iCount is the number of items in the pList
================================= */
client_sprite_t *GetSpriteList( client_sprite_t *pList, const char *psz, int iRes, int iCount )
{
	if( !pList )
		return NULL;

	int i = iCount;
	client_sprite_t *p = pList;

	while( i-- )
	{
		if( ( !strcmp( psz, p->szName ) ) && ( p->iRes == iRes ) )
			return p;
		p++;
	}

	return NULL;
}

void ScaleColors( int& r, int& g, int& b, const int a )
{
	const float x = ( float ) a / 255;
	r = ( int ) ( r * x );
	g = ( int ) ( g * x );
	b = ( int ) ( b * x );
}

bool UTIL_GetPlayerUniqueID( int iPlayer, char playerID[ PLAYERID_BUFFER_SIZE ] )
{
	return !!gEngfuncs.GetPlayerUniqueID( iPlayer, playerID );
}