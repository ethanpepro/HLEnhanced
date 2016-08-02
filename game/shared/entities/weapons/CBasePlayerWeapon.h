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
#ifndef GAME_SHARED_ENTITIES_WEAPONS_CBASEPLAYERWEAPON_H
#define GAME_SHARED_ENTITIES_WEAPONS_CBASEPLAYERWEAPON_H

#include "WeaponsConst.h"

#include "CWeaponInfo.h"

// Items that the player has in their inventory that they can use
class CBasePlayerWeapon : public CBaseAnimating
{
public:
	DECLARE_CLASS( CBasePlayerWeapon, CBaseAnimating );
	DECLARE_DATADESC();

	CBasePlayerWeapon( const int iID )
		: m_iId( iID )
	{
	}

	void Precache() override;

	virtual void SetObjectCollisionBox( void ) override;

	virtual bool AddToPlayer( CBasePlayer *pPlayer ); // return true if the item you want the item added to the player inventory
	virtual bool AddDuplicate( CBasePlayerWeapon *pItem ); // return true if you want your duplicate removed from world

	void EXPORT DestroyItem( void );
	void EXPORT DefaultTouch( CBaseEntity *pOther );	// default weapon touch
	void EXPORT FallThink( void );// when an item is first spawned, this think is run to determine when the object has hit the ground.
	void EXPORT Materialize( void );// make a weapon visible and tangible
	void EXPORT AttemptToMaterialize( void );  // the weapon desires to become visible and tangible, if the game rules allow for it

	CBaseEntity* Respawn( void ) override;// copy a weapon
	void FallInit( void );
	void CheckRespawn( void );

	virtual bool ExtractAmmo( CBasePlayerWeapon *pWeapon ); //{ return true; }			// Return true if you can add ammo to yourself when picked up
	virtual bool ExtractClipAmmo( CBasePlayerWeapon *pWeapon );// { return true; }			// Return true if you can add ammo to yourself when picked up

	virtual bool AddWeapon() { ExtractAmmo( this ); return true; }	// Return true if you want to add yourself to the player

	// generic "shared" ammo handlers
	bool AddPrimaryAmmo( int iCount, const char *szName, int iMaxClip );
	bool AddSecondaryAmmo( int iCount, const char *szName );

	virtual void UpdateItemInfo( void ) {}	// updates HUD state

	bool m_bPlayEmptySound;
	bool m_bFireOnEmpty;		// True when the gun is empty and the player is still holding down the
								// attack key(s)
	virtual bool PlayEmptySound();
	virtual void ResetEmptySound();

	virtual void SendWeaponAnim( int iAnim, int skiplocal = 1, int body = 0 );  // skiplocal is 1 if client is predicting weapon animations

	virtual bool CanDeploy() const;
	// returns if deploy was successful
	virtual bool Deploy() { return true; }

	virtual bool CanHolster() { return true; } // can this weapon be put away right now?

	virtual void Drop( void );
	virtual void Kill( void );
	virtual void AttachToPlayer( CBasePlayer *pPlayer );

	virtual bool IsUseable();
	bool DefaultDeploy( const char* const pszViewModel, const char* const pszWeaponModel, int iAnim, const char* const pszAnimExt, int skiplocal = 0, int body = 0 );
	bool DefaultReload( int iAnim, float fDelay, int body = 0 );

	virtual void ItemPreFrame( void ) { return; }		// called each frame by the player PreThink
	virtual void ItemPostFrame( void );					// called each frame by the player PostThink

	// called by CBasePlayerWeapons ItemPostFrame()
	virtual void PrimaryAttack( void ) { return; }				// do "+ATTACK"
	virtual void SecondaryAttack( void ) { return; }			// do "+ATTACK2"
	virtual void Reload( void ) { return; }						// do "+RELOAD"
	virtual void WeaponIdle( void ) { return; }					// called when no buttons pressed
	virtual bool UpdateClientData( CBasePlayer *pPlayer );		// sends hud info to client dll, if things have changed
	virtual void RetireWeapon( void );
	virtual bool ShouldWeaponIdle() { return false; }
	virtual void Holster( int skiplocal = 0 );
	virtual bool UseDecrement() const { return false; }

	void PrintState( void );

	float GetNextAttackDelay( float delay );

	/**
	*	Copies weapon data to the weapon_data_t instance.
	*	@param data Weapon data.
	*/
	virtual void GetWeaponData( weapon_data_t& data );

	/**
	*	Copies weapon data from the weapon_data_t instance.
	*	@param data Weapon data.
	*/
	virtual void SetWeaponData( const weapon_data_t& data );

	int PrimaryAmmoIndex() const;
	int SecondaryAmmoIndex() const;

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

	CBasePlayer	*m_pPlayer;
	CBasePlayerWeapon *m_pNext;

	const int m_iId; // WEAPON_???

	float	m_flNextPrimaryAttack;								// soonest time ItemPostFrame will call PrimaryAttack
	float	m_flNextSecondaryAttack;							// soonest time ItemPostFrame will call SecondaryAttack
	float	m_flTimeWeaponIdle;									// soonest time ItemPostFrame will call WeaponIdle
	int		m_iClip;											// number of shots left in the primary weapon clip, -1 it not used
	int		m_iClientClip;										// the last version of m_iClip sent to hud dll
	WpnOnTargetState m_iClientWeaponState;						// the last version of the weapon state sent to hud dll (is current weapon, is on target)
	bool	m_fInReload;										// Are we in the middle of a reload;

	int		m_iDefaultAmmo;// how much ammo you get when you pick up this weapon as placed by a level designer.

						   // hle time creep vars
	float	m_flPrevPrimaryAttack;
	float	m_flLastFireTime;

private:
	const CWeaponInfo* m_pWeaponInfo = nullptr;
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CBASEPLAYERWEAPON_H