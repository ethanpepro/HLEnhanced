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
#ifndef GAME_SERVER_MAPCYCLE_H
#define GAME_SERVER_MAPCYCLE_H

#define MAX_RULE_BUFFER 1024

struct mapcycle_item_t
{
	mapcycle_item_t *next;

	char mapname[ 32 ];
	int  minplayers, maxplayers;
	char rulebuffer[ MAX_RULE_BUFFER ];
};

struct mapcycle_t
{
	mapcycle_item_t *items;
	mapcycle_item_t *next_item;
};

/*
==============
ReloadMapCycleFile


Parses mapcycle.txt file into mapcycle_t structure
==============
*/
int ReloadMapCycleFile( char *filename, mapcycle_t *cycle );

/*
==============
DestroyMapCycle

Clean up memory used by mapcycle when switching it
==============
*/
void DestroyMapCycle( mapcycle_t *cycle );

#endif //GAME_SERVER_MAPCYCLE_H
