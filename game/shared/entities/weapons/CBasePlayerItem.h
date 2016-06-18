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

// Items that the player has in their inventory that they can use
class CBasePlayerItem : public CBaseAnimating
{
public:
	DECLARE_CLASS( CBasePlayerItem, CBaseAnimating );
	DECLARE_DATADESC();

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
	virtual bool GetItemInfo( ItemInfo* p ) { return false; } // returns false if struct not filled out
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

	virtual int PrimaryAmmoIndex() const { return -1; }
	virtual int SecondaryAmmoIndex() const { return -1; }

	virtual bool UpdateClientData( CBasePlayer *pPlayer ) { return false; }

	virtual CBasePlayerItem *GetWeaponPtr( void ) { return NULL; };

	static ItemInfo ItemInfoArray[ MAX_WEAPONS ];

	CBasePlayer	*m_pPlayer;
	CBasePlayerItem *m_pNext;
	int		m_iId;												// WEAPON_???

	virtual int iItemSlot( void ) { return 0; }			// return 0 to MAX_ITEMS_SLOTS, used in hud

	int			iItemPosition() const { return ItemInfoArray[ m_iId ].iPosition; }
	const char	*pszAmmo1() const { return ItemInfoArray[ m_iId ].pszAmmo1; }
	const char	*pszAmmo2() const { return ItemInfoArray[ m_iId ].pszAmmo2; }
	const char	*pszName() const { return ItemInfoArray[ m_iId ].pszName; }
	int			iMaxClip() const { return ItemInfoArray[ m_iId ].iMaxClip; }
	int			iWeight() const { return ItemInfoArray[ m_iId ].iWeight; }
	int			iFlags() const { return ItemInfoArray[ m_iId ].iFlags; }

	// int		m_iIdPrimary;										// Unique Id for primary ammo
	// int		m_iIdSecondary;										// Unique Id for secondary ammo
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CBASEPLAYERITEM_H