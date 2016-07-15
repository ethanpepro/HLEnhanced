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
#ifndef GAME_SERVER_CWORLD_H
#define GAME_SERVER_CWORLD_H

#define SF_WORLD_DARK		0x0001		// Fade from black at startup
#define SF_WORLD_TITLE		0x0002		// Display game title at startup
#define SF_WORLD_FORCETEAM	0x0004		// Force teams

// this moved here from world.cpp, to allow classes to be derived from it
//=======================
// CWorld
//
// This spawns first when each level begins.
//=======================
class CWorld : public CBaseEntity
{
public:
	DECLARE_CLASS( CWorld, CBaseEntity );

	void OnCreate() override;

	void OnDestroy() override;

	void Spawn( void ) override;
	void Precache( void ) override;
	void KeyValue( KeyValueData *pkvd ) override;

	/**
	*	Gets the global instance of this entity.
	*/
	static CWorld* GetInstance()
	{
		return m_pInstance;
	}

private:
	static CWorld* m_pInstance;

	char m_szMapScript[ MAX_PATH ] = { '\0' };
};

#endif //GAME_SERVER_CWORLD_H