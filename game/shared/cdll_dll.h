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
//
//  cdll_dll.h

// this file is included by both the game-dll and the client-dll,

#ifndef GAME_SHARED_CDLL_DLL_H
#define GAME_SHARED_CDLL_DLL_H

/**
*	The maximum number of weapons that the engine can network. This is defined as 32 in the SDK, but can handle up to 64.
*	The weapons bitmask can only handle 32. Either use an iuser or another means of sending the weapon list.
*	 - Solokiller
*/
#define MAX_WEAPONS		64		// ???

/**
*	Hud item selection slots.
*	Now up to 10.
*/
#define MAX_WEAPON_SLOTS		10
//TODO: this shouldn't even exist. The server and client indices should be identical. - Solokiller
#define MAX_ITEM_TYPES			11	// hud item selection slots

#define MAX_ITEMS				5	// hard coded item types

#define	HIDEHUD_WEAPONS		( 1<<0 )
#define	HIDEHUD_FLASHLIGHT	( 1<<1 )
#define	HIDEHUD_ALL			( 1<<2 )
#define 	HIDEHUD_HEALTH		( 1<<3 )

#define	MAX_AMMO_TYPES	32		// ???
#define MAX_AMMO_SLOTS  32		// not really slots

#define HUD_PRINTNOTIFY		1
#define HUD_PRINTCONSOLE	2
#define HUD_PRINTTALK		3
#define HUD_PRINTCENTER		4


#define WEAPON_SUIT			31

/**
*	Maximum world coordinate for any axis. Negate for the opposite end of the axis.
*	TODO: If the maximum map size were ever increased, this should be updated as well. Editor and compiler hacks can allow for up to 16384, so account for that.
*/
#define WORLD_BOUNDARY 16384

/**
*	Maximum velocity for any axis. Used for validation only.
*	TODO: probably not the best way to handle this, seeing as sv_maxvelocity can be higher. - Solokiller
*/
#define MAX_VELOCITY 2000

#endif //GAME_SHARED_CDLL_DLL_H
