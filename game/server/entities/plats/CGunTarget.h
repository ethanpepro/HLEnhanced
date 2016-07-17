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
#ifndef GAME_SERVER_ENTITIES_PLATS_CGUNTARGET_H
#define GAME_SERVER_ENTITIES_PLATS_CGUNTARGET_H

#define FGUNTARGET_START_ON			0x0001

// ----------------------------------------------------------
//
//
// pev->speed is the travel speed
// pev->health is current health
// pev->max_health is the amount to reset to each time it starts
class CGunTarget : public CBaseMonster
{
public:
	DECLARE_CLASS( CGunTarget, CBaseMonster );
	DECLARE_DATADESC();

	void			Spawn() override;
	void			Activate() override;
	void EXPORT		Next();
	void EXPORT		Start();
	void EXPORT		Wait();
	void			Stop() override;

	int				BloodColor() const override { return DONT_BLEED; }
	int				Classify() override { return CLASS_MACHINE; }
	void			OnTakeDamage( const CTakeDamageInfo& info ) override;
	void			Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	Vector			BodyTarget( const Vector &posSrc ) const override { return pev->origin; }

	virtual int	ObjectCaps() const override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

private:
	bool			m_on;
};

#endif //GAME_SERVER_ENTITIES_PLATS_CGUNTARGET_H