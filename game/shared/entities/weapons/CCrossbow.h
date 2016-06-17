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
#ifndef GAME_SHARED_ENTITIES_WEAPONS_CCROSSBOW_H
#define GAME_SHARED_ENTITIES_WEAPONS_CCROSSBOW_H

class CCrossbow : public CBasePlayerWeapon
{
public:
	DECLARE_CLASS( CCrossbow, CBasePlayerWeapon );

	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot() override { return 3; }
	bool GetItemInfo( ItemInfo* p ) override;

	void FireBolt( void );
	void FireSniperBolt( void );
	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	bool AddToPlayer( CBasePlayer *pPlayer ) override;
	bool Deploy() override;
	void Holster( int skiplocal = 0 ) override;
	void Reload( void ) override;
	void WeaponIdle( void ) override;

	int m_fInZoom; // don't save this

	virtual bool UseDecrement() const override
	{
#if defined( CLIENT_WEAPONS )
		return true;
#else
		return false;
#endif
	}

private:
	unsigned short m_usCrossbow;
	unsigned short m_usCrossbow2;
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CCROSSBOW_H