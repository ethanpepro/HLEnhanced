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
#ifndef GAME_SHARED_ENTITIES_WEAPONS_CBASEPLAYERITEM_H
#define GAME_SHARED_ENTITIES_WEAPONS_CBASEPLAYERITEM_H

#include "CWeaponInfo.h"

/**
*	Flag values for CWeaponData::m_iFlags
*	@see CWeaponData
*/
enum ItemInfoFlag
{
	ITEM_FLAG_NONE			= 0,
	/**
	*	Not used.
	*	TODO: should be repurposed for replenishing weapons (e.g. hornet gun) - Solokiller
	*/
	ITEM_FLAG_SELECTONEMPTY = 1,

	/**
	*	Don't automatically reload this weapon.
	*/
	ITEM_FLAG_NOAUTORELOAD = 2,

	/**
	*	Don't automatically switch to another weapon if this weapon is empty.
	*/
	ITEM_FLAG_NOAUTOSWITCHEMPTY = 4,

	/**
	*	If the entity count is nearing the maximum, this entity's respawn will be delayed.
	*	Multiplayer only.
	*/
	ITEM_FLAG_LIMITINWORLD = 8,

	/**
	*	A player can totally exhaust their ammo supply and lose this weapon.
	*/
	ITEM_FLAG_EXHAUSTIBLE = 16,
};

/**
*	Converts the given item info flag to a string representation.
*/
const char* ItemInfoFlagToString( const ItemInfoFlag flag );

/**
*	Converts the given string to an item info flag.
*	@return Item info flag, or ITEM_FLAG_NONE if the string didn't match any other constant.
*/
ItemInfoFlag StringToItemInfoFlag( const char* const pszString );

#define WEAPON_IS_ONTARGET 0x40

class CBasePlayerWeapon;

// Items that the player has in their inventory that they can use
class CBasePlayerItem : public CBaseAnimating
{
public:
	DECLARE_CLASS( CBasePlayerItem, CBaseAnimating );
	DECLARE_DATADESC();

	/**
	*	Constructor.
	*	@param iID The weapon ID.
	*/
	CBasePlayerItem( const int iID );

	/**
	*	Must be called by subclasses.
	*/
	void Precache() override;

	virtual void SetObjectCollisionBox( void ) override;

	virtual bool AddToPlayer( CBasePlayer *pPlayer );	// return true if the item you want the item added to the player inventory
	virtual bool AddDuplicate( CBasePlayerItem *pItem ) { return false; }	// return true if you want your duplicate removed from world
	void EXPORT DestroyItem( void );
	void EXPORT DefaultTouch( CBaseEntity *pOther );	// default weapon touch
	void EXPORT FallThink( void );// when an item is first spawned, this think is run to determine when the object has hit the ground.
	void EXPORT Materialize( void );// make a weapon visible and tangible
	void EXPORT AttemptToMaterialize( void );  // the weapon desires to become visible and tangible, if the game rules allow for it
	CBaseEntity* Respawn( void ) override;// copy a weapon
	void FallInit( void );
	void CheckRespawn( void );
	virtual bool CanDeploy() const { return true; }
	// returns if deploy was successful
	virtual bool Deploy() { return true; }

	virtual bool CanHolster() { return true; } // can this weapon be put away right now?
	virtual void Holster( int skiplocal = 0 );
	virtual void UpdateItemInfo( void ) { return; };

	virtual void ItemPreFrame( void ) { return; }		// called each frame by the player PreThink
	virtual void ItemPostFrame( void ) { return; }		// called each frame by the player PostThink

	virtual void Drop( void );
	virtual void Kill( void );
	virtual void AttachToPlayer( CBasePlayer *pPlayer );

	int PrimaryAmmoIndex() const;
	int SecondaryAmmoIndex() const;

	virtual bool UpdateClientData( CBasePlayer *pPlayer ) { return false; }

	virtual CBasePlayerWeapon* GetWeaponPtr() { return nullptr; }

	CBasePlayer	*m_pPlayer;
	CBasePlayerItem *m_pNext;
	const int m_iId;												// WEAPON_???

	/**
	*	@eturn 0 to MAX_ITEMS_SLOTS, used in hud.
	*/
	int			iItemSlot() const { return m_pWeaponInfo->GetBucket(); }
	int			iItemPosition() const { return m_pWeaponInfo->GetPosition(); }
	const char	*pszAmmo1() const { return m_pWeaponInfo->GetPrimaryAmmoName(); }
	const char	*pszAmmo2() const { return m_pWeaponInfo->GetSecondaryAmmoName(); }
	const char	*pszName() const { return m_pWeaponInfo->GetWeaponName(); }
	int			iMaxClip() const { return m_pWeaponInfo->GetMaxMagazine(); }
	int			iWeight() const { return m_pWeaponInfo->GetWeight(); }
	int			iFlags() const { return m_pWeaponInfo->GetFlags(); }

	const CWeaponInfo* GetWeaponInfo() const { return m_pWeaponInfo; }

private:
	const CWeaponInfo* m_pWeaponInfo = nullptr;
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CBASEPLAYERITEM_H