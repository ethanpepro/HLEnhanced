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
#ifndef GAME_SHARED_GAMECONSTANTS_H
#define GAME_SHARED_GAMECONSTANTS_H

/**
*	Approx 60 feet.
*/
#define PLAYER_FATAL_FALL_SPEED 1024

/**
*	Approx 20 feet.
*/
#define PLAYER_MAX_SAFE_FALL_SPEED 580

/**
*	Damage per unit per second.
*/
#define DAMAGE_FOR_FALL_SPEED ( ( float ) 100 / ( PLAYER_FATAL_FALL_SPEED - PLAYER_MAX_SAFE_FALL_SPEED ) )

#define PLAYER_MIN_BOUNCE_SPEED 200

/**
*	 Won't punch player's screen/make scrape noise unless player falling at least this fast.
*/
#define PLAYER_FALL_PUNCH_THRESHHOLD ( ( float ) 350 )

/**
*	Fastest vertical climbing speed possible.
*/
#define	MAX_CLIMB_SPEED 200

/**
*	How fast we longjump.
*/
#define PLAYER_LONGJUMP_SPEED 350

/**
*	Minimum horizontal velocity that a player must be moving at before step sounds are played.
*/
#define PLAYER_STEP_SOUND_SPEED 220

#endif //GAME_SHARED_GAMECONSTANTS_H