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
#ifndef GAME_SERVER_CBASEDOOR_H
#define GAME_SERVER_CBASEDOOR_H

class CBaseDoor : public CBaseToggle
{
public:
	DECLARE_CLASS( CBaseDoor, CBaseToggle );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;
	virtual void KeyValue( KeyValueData *pkvd ) override;
	virtual void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	virtual void Blocked( CBaseEntity *pOther ) override;


	virtual int	ObjectCaps() const override
	{
		if( pev->spawnflags & SF_ITEM_USE_ONLY )
			return ( CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION ) | FCAP_IMPULSE_USE;
		else
			return ( CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION );
	}

	virtual void SetToggleState( int state ) override;

	// used to selectivly override defaults
	void EXPORT DoorTouch( CBaseEntity *pOther );

	// local functions
	int DoorActivate();
	void EXPORT DoorGoUp( void );
	void EXPORT DoorGoDown( void );
	void EXPORT DoorHitTop( void );
	void EXPORT DoorHitBottom( void );

	byte	m_bHealthValue;// some doors are medi-kit doors, they give players health

	byte	m_bMoveSnd;			// sound a door makes while moving
	byte	m_bStopSnd;			// sound a door makes when it stops

	locksound_t m_ls;			// door lock sounds

	byte	m_bLockedSound;		// ordinals from entity selection
	byte	m_bLockedSentence;
	byte	m_bUnlockedSound;
	byte	m_bUnlockedSentence;
};

#define noiseMoving noise1
#define noiseArrived noise2

#endif //GAME_SERVER_CBASEDOOR_H