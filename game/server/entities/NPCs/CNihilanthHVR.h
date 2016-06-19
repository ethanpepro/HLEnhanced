/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#ifndef GAME_SERVER_ENTITIES_NPCS_CNIHILANTHHVR_H
#define GAME_SERVER_ENTITIES_NPCS_CNIHILANTHHVR_H

class CNihilanth;

//=========================================================
// Controller bouncy ball attack
//=========================================================
class CNihilanthHVR : public CBaseMonster
{
public:
	DECLARE_CLASS( CNihilanthHVR, CBaseMonster );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	void Precache( void ) override;

	void CircleInit( CBaseEntity *pTarget );
	void AbsorbInit( void );
	void TeleportInit( CNihilanth *pOwner, CBaseEntity *pEnemy, CBaseEntity *pTarget, CBaseEntity *pTouch );
	void GreenBallInit( void );
	void ZapInit( CBaseEntity *pEnemy );

	void EXPORT HoverThink( void );
	bool CircleTarget( Vector vecTarget );
	void EXPORT DissipateThink( void );

	void EXPORT ZapThink( void );
	void EXPORT TeleportThink( void );
	void EXPORT TeleportTouch( CBaseEntity *pOther );

	void EXPORT RemoveTouch( CBaseEntity *pOther );
	void EXPORT BounceTouch( CBaseEntity *pOther );
	void EXPORT ZapTouch( CBaseEntity *pOther );

	//TODO: refactor out of class - Solokiller
	CBaseEntity *RandomClassname( const char *szName );

	// void EXPORT SphereUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	void MovetoTarget( Vector vecTarget );
	virtual void Crawl( void );

	void Zap( void );
	void Teleport( void );

	float m_flIdealVel;
	Vector m_vecIdeal;
	CNihilanth *m_pNihilanth;
	EHANDLE m_hTouch;
	int m_nFrames;
};

#endif //GAME_SERVER_ENTITIES_NPCS_CNIHILANTHHVR_H