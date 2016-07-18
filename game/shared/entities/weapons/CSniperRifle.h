#ifndef GAME_SHARED_ENTITIES_WEAPONS_CSNIPERRIFLE_H
#define GAME_SHARED_ENTITIES_WEAPONS_CSNIPERRIFLE_H

enum SniperRifleAnim
{
	SNIPERRIFLE_DRAW = 0,
	SNIPERRIFLE_SLOWIDLE,
	SNIPERRIFLE_FIRE,
	SNIPERRIFLE_FIRELASTROUND,
	SNIPERRIFLE_RELOAD1,
	SNIPERRIFLE_RELOAD2,
	SNIPERRIFLE_RELOAD3,
	SNIPERRIFLE_SLOWIDLE2,
	SNIPERRIFLE_HOLSTER
};

/**
*	Opposing force sniper rifle.
*	TODO: use base class for snipers?
*	- Solokiller
*/
class CSniperRifle : public CBasePlayerWeapon
{
public:
	DECLARE_CLASS( CSniperRifle, CBasePlayerWeapon );
	DECLARE_DATADESC();

	void Precache() override;
	void Spawn() override;

	bool GetItemInfo( ItemInfo* pInfo ) override;

	bool AddToPlayer( CBasePlayer* pPlayer ) override;

	bool Deploy() override;

	void Holster( int skiplocal = 0 ) override;

	void WeaponIdle() override;

	void PrimaryAttack() override;

	void SecondaryAttack() override;

	void Reload() override;

	bool UseDecrement() const override
	{
#ifdef CLIENT_WEAPONS
		return true;
#else
		return false;
#endif
	}

	void ToggleZoom();

private:
	unsigned short m_usSniper;

	bool m_bInZoom;
	bool m_bReloading;
	float m_flReloadStart;
};

#endif //GAME_SHARED_ENTITIES_WEAPONS_CSNIPERRIFLE_H