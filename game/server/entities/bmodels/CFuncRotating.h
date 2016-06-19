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
#ifndef GAME_SERVER_ENTITIES_BMODELS_CFUNCROTATING_H
#define GAME_SERVER_ENTITIES_BMODELS_CFUNCROTATING_H

#define		SF_BRUSH_ACCDCC	16// brush should accelerate and decelerate when toggled
#define		SF_BRUSH_HURT		32// rotating brush that inflicts pain based on rotation speed
#define		SF_ROTATING_NOT_SOLID	64	// some special rotating objects are not solid.

// covering cheesy noise1, noise2, & noise3 fields so they make more sense (for rotating fans)
#define		noiseStart		noise1
#define		noiseStop		noise2
#define		noiseRunning	noise3

#define FANPITCHMIN		30
#define FANPITCHMAX		100

/*QUAKED func_rotating (0 .5 .8) ? START_ON REVERSE X_AXIS Y_AXIS
You need to have an origin brush as part of this entity.  The
center of that brush will be
the point around which it is rotated. It will rotate around the Z
axis by default.  You can
check either the X_AXIS or Y_AXIS box to change that.

"speed" determines how fast it moves; default value is 100.
"dmg"	damage to inflict when blocked (2 default)

REVERSE will cause the it to rotate in the opposite direction.
*/
class CFuncRotating : public CBaseEntity
{
public:
	DECLARE_CLASS( CFuncRotating, CBaseEntity );
	DECLARE_DATADESC();

	// basic functions
	void Spawn( void ) override;
	void Precache( void ) override;
	void EXPORT SpinUp( void );
	void EXPORT SpinDown( void );
	void KeyValue( KeyValueData* pkvd ) override;
	void EXPORT HurtTouch( CBaseEntity *pOther );
	void EXPORT RotatingUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void EXPORT Rotate( void );
	void RampPitchVol( const bool bUp );
	void Blocked( CBaseEntity *pOther ) override;
	virtual int	ObjectCaps() const override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	float m_flFanFriction;
	float m_flAttenuation;
	float m_flVolume;
	float m_pitch;
	int	  m_sounds;
};

#endif //GAME_SERVER_ENTITIES_BMODELS_CFUNCROTATING_H