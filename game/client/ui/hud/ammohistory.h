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
// ammohistory.h
//

#include "entities/weapons/CAmmoTypes.h"
#include "CWeaponInfo.h"

struct WeaponHUDSprite;

class WeaponsResource
{
private:
	//Rather than dynamically allocate the array (and reallocate it a bunch on connect), just use the maximum. - Solokiller
	int riAmmo[ CAmmoTypes::MAX_AMMO_TYPES ];							// count of each ammo type

public:
	void Init();

	void Reset( void )
	{
		iOldWeaponBits = 0;
		memset( riAmmo, 0, sizeof( riAmmo ) );
	}

///// WEAPON /////
	int			iOldWeaponBits;

	void DropAllWeapons();

	CBasePlayerWeapon* GetWeaponSlot( int slot, int pos );

	CBasePlayerWeapon* GetFirstPos( int iSlot );
	void SelectSlot( int iSlot, const bool fAdvance, int iDirection );
	CBasePlayerWeapon* GetNextActivePos( int iSlot, int iSlotPos );

	bool HasAmmo( const CBasePlayerWeapon* const p ) const;

///// AMMO /////
	void SetAmmo( int iId, int iCount ) { riAmmo[ iId ] = iCount;	}

	int CountAmmo( int iId ) const;

	const WeaponHUDSprite* GetAmmoPicFromWeapon( int iAmmoId ) const;
};

extern WeaponsResource gWR;

//TODO: increase this? - Solokiller
#define MAX_HISTORY 12
enum {
	HISTSLOT_EMPTY,
	HISTSLOT_AMMO,
	HISTSLOT_WEAP,
	HISTSLOT_ITEM,
};

class HistoryResource
{
private:
	struct HIST_ITEM {
		int type;
		float DisplayTime;  // the time at which this item should be removed from the history
		int iCount;
		int iId;
	};

	HIST_ITEM rgAmmoHistory[MAX_HISTORY];

public:

	void Init();

	void Reset()
	{
		memset( rgAmmoHistory, 0, sizeof rgAmmoHistory );
	}

	int GetAmmoPickupGap() const { return iHistoryGap + 5; }

	int GetAmmoPickupPickHeight() const { return 32 + ( iHistoryGap * 2 ); }

	int GetAmmoPickupHeightMax() const { return ScreenHeight - 100; }

	int iHistoryGap;
	int iCurrentHistorySlot;

	void AddToHistory( int iType, int iId, int iCount = 0 );
	void AddToHistory( int iType, const char *szName, int iCount = 0 );

	void CheckClearHistory();
	int DrawAmmoHistory( float flTime );

private:
	cvar_t* m_phud_drawhistory_time = nullptr;
};

extern HistoryResource gHR;



