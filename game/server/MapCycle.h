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

//TODO: use max map name constant - Solokiller

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
*	Parses mapcycle.txt file into mapcycle_t structure.
*	@param pszFileName Name of the map cycle file to load.
*	@param pCycle Map cycle to fill.
*	@return Whether the file was successfully loaded or not.
*/
bool ReloadMapCycleFile( const char* pszFileName, mapcycle_t* pCycle );

/*
*	Clean up memory used by mapcycle when switching it.
*	@param pCycle Map cycle to clean up.
*/
void DestroyMapCycle( mapcycle_t* pCycle );

/*
*	Parse commands/key value pairs to issue right after map xxx command is issued on server
*	level transition
*/
void ExtractCommandString( char *s, char *szCommand );

/**
*	The map cycle.
*/
extern mapcycle_t g_MapCycle;

/**
*	Name of the previous map cycle that was loaded. If no map cycle has been loaded, this will be empty.
*/
extern char g_szPreviousMapCycleFile[ MAX_PATH ];

#endif //GAME_SERVER_MAPCYCLE_H
