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
#ifndef GAME_SHARED_ENTITIES_CBASECOMBATCHARACTER_H
#define GAME_SHARED_ENTITIES_CBASECOMBATCHARACTER_H

/**
*	Generic character.
*/
class CBaseCombatCharacter : public CBaseToggle
{
public:
	DECLARE_CLASS( CBaseCombatCharacter, CBaseToggle );

	virtual	Vector GetGunPosition() = 0;

	virtual bool ShouldFadeOnDeath() const;

	virtual void BarnacleVictimBitten( CBaseEntity* pBarnacle ) = 0;
	virtual void BarnacleVictimReleased() = 0;
};

#endif //GAME_SHARED_ENTITIES_CBASECOMBATCHARACTER_H
