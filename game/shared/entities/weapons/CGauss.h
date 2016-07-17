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
#ifndef GAME_SHARED_ENTITIES_WEAPONS_CGAUSS_H
#define GAME_SHARED_ENTITIES_WEAPONS_CGAUSS_H

enum gauss_e
{
	GAUSS_IDLE = 0,
	GAUSS_IDLE2,
	GAUSS_FIDGET,
	GAUSS_SPINUP,
	GAUSS_SPIN,
	GAUSS_FIRE,
	GAUSS_FIRE2,
	GAUSS_HOLSTER,
	GAUSS_DRAW
};

#define	GAUSS_PRIMARY_CHARGE_VOLUME	256// how loud gauss is while charging
#define GAUSS_PRIMARY_FIRE_VOLUME	450// how loud gauss is when discharged

class CGauss : public CBasePlayerWeapon
{
public:
	enum class AttackState
	{
		NOT_ATTACKING = 0,
		CHARGING_START = 1,
		CHARGING = 2,
	};

public:
	DECLARE_CLASS( CGauss, CBasePlayerWeapon );
#ifndef CLIENT_DLL
	DECLARE_DATADESC();
#endif

	void Spawn( void ) override;
	void Precache( void ) override;
	int iItemSlot( void ) override { return 4; }
	bool GetItemInfo( ItemInfo* p ) override;
	bool AddToPlayer( CBasePlayer *pPlayer ) override;

	bool Deploy() override;
	void Holster( int skiplocal = 0 ) override;

	void PrimaryAttack( void ) override;
	void SecondaryAttack( void ) override;
	void WeaponIdle( void ) override;

	void StartFire( void );
	void Fire( Vector vecOrigSrc, Vector vecDirShooting, float flDamage );
	float GetFullChargeTime( void );

	void GetWeaponData( weapon_data_t& data ) override
	{
		BaseClass::GetWeaponData( data );

		data.iuser2 = static_cast<int>( m_InAttack );
	}

	void SetWeaponData( const weapon_data_t& data ) override
	{
		BaseClass::SetWeaponData( data );

		m_InAttack = static_cast<AttackState>( data.iuser2 );
	}

	int m_iBalls;
	int m_iGlow;
	int m_iBeam;
	int m_iSoundState; // don't save this

					   // was this weapon just fired primary or secondary?
					   // we need to know so we can pick the right set of effects. 
	bool m_fPrimaryFire;

	virtual bool UseDecrement() const override
	{
#if defined( CLIENT_WEAPONS )
		return true;
#else
		return false;
#endif
	}

private:
	unsigned short m_usGaussFire;
	unsigned short m_usGaussSpin;

	AttackState m_InAttack;
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CGAUSS_H