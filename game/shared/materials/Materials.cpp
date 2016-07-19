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
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"

#include "Materials.h"

// Texture names
CMaterialsList g_MaterialsList;

char PM_FindTextureType( const char* pszName )
{
	return g_MaterialsList.FindTextureType( pszName );
}

bool GetMaterialForType( const char chTextureType, const int iBulletType, float& flVol, float& flVolBar, float& flAttn, const char** pszSounds, size_t& uiCount )
{
	flAttn = ATTN_NORM;

	switch( chTextureType )
	{
	default:
	case CHAR_TEX_CONCRETE: flVol = 0.9; flVolBar = 0.6;
		pszSounds[ 0 ] = "player/pl_step1.wav";
		pszSounds[ 1 ] = "player/pl_step2.wav";
		uiCount = 2;
		break;
	case CHAR_TEX_METAL: flVol = 0.9; flVolBar = 0.3;
		pszSounds[ 0 ] = "player/pl_metal1.wav";
		pszSounds[ 1 ] = "player/pl_metal2.wav";
		uiCount = 2;
		break;
	case CHAR_TEX_DIRT:	flVol = 0.9; flVolBar = 0.1;
		pszSounds[ 0 ] = "player/pl_dirt1.wav";
		pszSounds[ 1 ] = "player/pl_dirt2.wav";
		pszSounds[ 2 ] = "player/pl_dirt3.wav";
		uiCount = 3;
		break;
	case CHAR_TEX_VENT:	flVol = 0.5; flVolBar = 0.3;
		pszSounds[ 0 ] = "player/pl_duct1.wav";
		pszSounds[ 1 ] = "player/pl_duct1.wav";
		uiCount = 2;
		break;
	case CHAR_TEX_GRATE: flVol = 0.9; flVolBar = 0.5;
		pszSounds[ 0 ] = "player/pl_grate1.wav";
		pszSounds[ 1 ] = "player/pl_grate4.wav";
		uiCount = 2;
		break;
	case CHAR_TEX_TILE:	flVol = 0.8; flVolBar = 0.2;
		pszSounds[ 0 ] = "player/pl_tile1.wav";
		pszSounds[ 1 ] = "player/pl_tile3.wav";
		pszSounds[ 2 ] = "player/pl_tile2.wav";
		pszSounds[ 3 ] = "player/pl_tile4.wav";
		uiCount = 4;
		break;
	case CHAR_TEX_SLOSH: flVol = 0.9; flVolBar = 0.0;
		pszSounds[ 0 ] = "player/pl_slosh1.wav";
		pszSounds[ 1 ] = "player/pl_slosh3.wav";
		pszSounds[ 2 ] = "player/pl_slosh2.wav";
		pszSounds[ 3 ] = "player/pl_slosh4.wav";
		uiCount = 4;
		break;
	case CHAR_TEX_WOOD: flVol = 0.9; flVolBar = 0.2;
		pszSounds[ 0 ] = "debris/wood1.wav";
		pszSounds[ 1 ] = "debris/wood2.wav";
		pszSounds[ 2 ] = "debris/wood3.wav";
		uiCount = 3;
		break;
	case CHAR_TEX_GLASS:
	case CHAR_TEX_COMPUTER:
		flVol = 0.8; flVolBar = 0.2;
		pszSounds[ 0 ] = "debris/glass1.wav";
		pszSounds[ 1 ] = "debris/glass2.wav";
		pszSounds[ 2 ] = "debris/glass3.wav";
		uiCount = 3;
		break;
	case CHAR_TEX_FLESH:
		if( iBulletType == BULLET_PLAYER_CROWBAR )
			return false; // crowbar already makes this sound
		flVol = 1.0;	flVolBar = 0.2;
		pszSounds[ 0 ] = "weapons/bullet_hit1.wav";
		pszSounds[ 1 ] = "weapons/bullet_hit2.wav";
		flAttn = 1.0;
		uiCount = 2;
		break;
	}

	return true;
}