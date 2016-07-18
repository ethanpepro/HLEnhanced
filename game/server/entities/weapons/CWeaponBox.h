#ifndef GAME_SERVER_ENTITIES_WEAPONS_CWEAPONBOX_H
#define GAME_SERVER_ENTITIES_WEAPONS_CWEAPONBOX_H

//=========================================================
// CWeaponBox - a single entity that can store weapons
// and ammo. 
//=========================================================
class CWeaponBox : public CBaseEntity
{
public:
	DECLARE_CLASS( CWeaponBox, CBaseEntity );
	DECLARE_DATADESC();

	void Precache() override;
	void Spawn() override;
	void Touch( CBaseEntity *pOther ) override;
	void KeyValue( KeyValueData *pkvd ) override;

private:
	bool IsEmpty() const;
	int  GiveAmmo( int iCount, char *szName, int iMax, int *pIndex = NULL );
	void SetObjectCollisionBox() override;

public:
	void EXPORT Kill();

	bool HasWeapon( CBasePlayerItem *pCheckItem ) const;
	bool PackWeapon( CBasePlayerItem *pWeapon );
	bool PackAmmo( int iszName, int iCount );

	CBasePlayerItem	*m_rgpPlayerItems[ MAX_WEAPON_SLOTS ];// one slot for each 

	int m_rgiszAmmo[ MAX_AMMO_SLOTS ];// ammo names
	int	m_rgAmmo[ MAX_AMMO_SLOTS ];// ammo quantities

	int m_cAmmoTypes;// how many ammo types packed into this box (if packed by a level designer)
};

#endif //GAME_SERVER_ENTITIES_WEAPONS_CWEAPONBOX_H