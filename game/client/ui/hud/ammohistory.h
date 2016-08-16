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

// this is the max number of items in each bucket
#define MAX_WEAPON_POSITIONS		MAX_WEAPONS

class WeaponsResource
{
private:
	// Information about weapons & ammo
	WEAPON		rgWeapons[MAX_WEAPONS];	// Weapons Array

	// counts of weapons * ammo
	WEAPON*		rgSlots[MAX_WEAPON_SLOTS+1][MAX_WEAPON_POSITIONS+1];	// The slots currently in use by weapons.  The value is a pointer to the weapon;  if it's NULL, no weapon is there

	//Rather than dynamically allocate the array (and reallocate it a bunch on connect), just use the maximum. - Solokiller
	int riAmmo[ CAmmoTypes::LAST_VALID_ID + 1 ];							// count of each ammo type

public:
	void Init();

	void Reset( void )
	{
		iOldWeaponBits = 0;
		memset( rgSlots, 0, sizeof( rgSlots ) );
		memset( riAmmo, 0, sizeof( riAmmo ) );
	}

	void VidInit();

///// WEAPON /////
	int			iOldWeaponBits;

	WEAPON *GetWeapon( int iId ) { return &rgWeapons[iId]; }
	void AddWeapon( WEAPON *wp ) 
	{ 
		ASSERT( wp );
		ASSERT( wp->pInfo );

		//TODO: this really needs to go - Solokiller
		rgWeapons[ wp->pInfo->GetID() ].pInfo = wp->pInfo;	
	}

	void PickupWeapon( WEAPON *wp )
	{
		ASSERT( wp );
		ASSERT( wp->pInfo );

		rgSlots[ wp->pInfo->GetBucket() ][ wp->pInfo->GetPosition() ] = wp;
	}

	void DropWeapon( WEAPON *wp )
	{
		ASSERT( wp );
		ASSERT( wp->pInfo );

		rgSlots[ wp->pInfo->GetBucket() ][ wp->pInfo->GetPosition() ] = NULL;
	}

	void DropAllWeapons( void )
	{
		for ( int i = 0; i < MAX_WEAPONS; i++ )
		{
			if ( rgWeapons[i].pInfo )
				DropWeapon( &rgWeapons[i] );
		}
	}

	WEAPON* GetWeaponSlot( int slot, int pos ) { return rgSlots[slot][pos]; }

	WEAPON* GetFirstPos( int iSlot );
	void SelectSlot( int iSlot, const bool fAdvance, int iDirection );
	WEAPON* GetNextActivePos( int iSlot, int iSlotPos );

	bool HasAmmo( const WEAPON* const p ) const;

///// AMMO /////
	void SetAmmo( int iId, int iCount ) { riAmmo[ iId ] = iCount;	}

	int CountAmmo( int iId ) const;

	const WeaponHUDSprite* GetAmmoPicFromWeapon( int iAmmoId ) const;

	/**
	*	Synchronize the weapons resource with the client side weapons list.
	*/
	void SyncWithWeapons();
};

extern WeaponsResource gWR;


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

	void Init( void )
	{
		Reset();
	}

	void Reset( void )
	{
		memset( rgAmmoHistory, 0, sizeof rgAmmoHistory );
	}

	int iHistoryGap;
	int iCurrentHistorySlot;

	void AddToHistory( int iType, int iId, int iCount = 0 );
	void AddToHistory( int iType, const char *szName, int iCount = 0 );

	void CheckClearHistory( void );
	int DrawAmmoHistory( float flTime );
};

extern HistoryResource gHR;



