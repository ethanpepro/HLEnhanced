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

/**
*	@defgroup AutoAimDegrees AutoAim Degrees
*
*	AutoAim degrees for use in weapons code.
*
*	Calculated as follows:
*	UTIL_MakeVectors( Vector( 0, 0, 0 ) );
*
*	AngleVectors( Vector( degrees, 0, 0 ), outVector, nullptr, nullptr );
*
*	double dot = fabs( DotProduct( outVector, gpGlobals->v_right ) )
*		+ fabs( DotProduct( outVector, gpGlobals->v_up ) ) * 0.5;
*
*	const double dot2 = dot * 2;
*
*	Use doubles when calculating for inproved accuracy.
*
*	@{
*/
#define AUTOAIM_1DEGREE		0.01745240643728
#define AUTOAIM_2DEGREES	0.03489949670250
#define AUTOAIM_3DEGREES	0.05233595624294
#define AUTOAIM_4DEGREES	0.06975647374413
#define AUTOAIM_5DEGREES	0.08715574274766
#define AUTOAIM_6DEGREES	0.10452846326765
#define AUTOAIM_7DEGREES	0.12186934340515
#define AUTOAIM_8DEGREES	0.13917310096007
#define AUTOAIM_9DEGREES	0.15643446504023
#define AUTOAIM_10DEGREES	0.17364817766690

/** @} */

#endif //GAME_SHARED_GAMECONSTANTS_H