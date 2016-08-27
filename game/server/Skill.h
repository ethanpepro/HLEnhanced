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
#ifndef GAME_SERVER_SKILL_H
#define GAME_SERVER_SKILL_H
//=========================================================
// skill.h - skill level concerns
//=========================================================

enum SkillLevel
{
	SKILL_FIRST		= 1,

	SKILL_EASY		= SKILL_FIRST,
	SKILL_MEDIUM	= 2,
	SKILL_HARD		= 3,

	SKILL_LAST		= SKILL_HARD
};

//TODO: refer to the cvars directly when quering for values - Solokiller
struct skilldata_t
{
	//TODO: temporary until the skill system is overhauled. - Solokiller
	friend class CHalfLifeMultiplay;

	/**
	*	@return The current skill level.
	*	@see SkillLevel
	*/
	SkillLevel GetSkillLevel() const { return m_SkillLevel; }

	/**
	*	Sets the current skill level.
	*/
	void SetSkillLevel( const SkillLevel skillLevel )
	{
		m_SkillLevel = skillLevel;
	}

	/**
	*	Gets the value for the given skill cvar.
	*	@return Value for the given skill cvar.
	*/
	float GetSkillCvar( const char* pszName ) const;

	/**
	*	Refreshes current skill data.
	*/
	void RefreshSkillData();

private:
	SkillLevel m_SkillLevel; // game skill level

	// Monster Health & Damage
	float agruntHealth;
	float agruntDmgPunch;

	float apacheHealth;

	float barneyHealth;

	float bigmommaHealthFactor;
	float bigmommaDmgSlash;
	float bigmommaDmgBlast;
	float bigmommaRadiusBlast;

	float bullsquidHealth;
	float bullsquidDmgBite;
	float bullsquidDmgWhip;
	float bullsquidDmgSpit;

	float gargantuaHealth;
	float gargantuaDmgSlash;
	float gargantuaDmgFire;
	float gargantuaDmgStomp;

	float hassassinHealth;

	float headcrabHealth;
	float headcrabDmgBite;

	float hgruntHealth;
	float hgruntDmgKick;
	float hgruntShotgunPellets;
	float hgruntGrenadeSpeed;

	float houndeyeHealth;
	float houndeyeDmgBlast;

	float slaveHealth;
	float slaveDmgClaw;
	float slaveDmgClawrake;
	float slaveDmgZap;

	float ichthyosaurHealth;
	float ichthyosaurDmgShake;

	float leechHealth;
	float leechDmgBite;

	float controllerHealth;
	float controllerDmgZap;
	float controllerSpeedBall;
	float controllerDmgBall;

	float nihilanthHealth;
	float nihilanthZap;

	float scientistHealth;

	float snarkHealth;
	float snarkDmgBite;
	float snarkDmgPop;

	float zombieHealth;
	float zombieDmgOneSlash;
	float zombieDmgBothSlash;

	float turretHealth;
	float miniturretHealth;
	float sentryHealth;

	// Player Weapons
	float plrDmgCrowbar;
	float plrDmg9MM;
	float plrDmg357;
	float plrDmgMP5;
	float plrDmgM203Grenade;
	float plrDmgBuckshot;
	float plrDmgCrossbowClient;
	float plrDmgCrossbowMonster;
	float plrDmgRPG;
	float plrDmgGauss;
	float plrDmgEgonNarrow;
	float plrDmgEgonWide;
	float plrDmgHornet;
	float plrDmgHandGrenade;
	float plrDmgSatchel;
	float plrDmgTripmine;

#if USE_OPFOR
	float plrDmgKnife;
	float plrDmgPipewrench;
	float plrDmgGrapple;
	float plrDmg556;
	float plrDmg762;
	float plrDmgDeagle;
	float plrDmgShockRoachS;
	float plrDmgShockRoachM;
	float plrDmgDisplacerOther;
	float plrRadiusDisplacer;
	float plrDmgSpore;
#endif

	// weapons shared by monsters
	float monDmg9MM;
	float monDmgMP5;
	float monDmg12MM;
	float monDmgHornet;

	// health/suit charge
	float suitchargerCapacity;
	float batteryCapacity;
	float healthchargerCapacity;
	float healthkitCapacity;
	float scientistHeal;

	// monster damage adj
	float monHead;
	float monChest;
	float monStomach;
	float monLeg;
	float monArm;

	// player damage adj
	float plrHead;
	float plrChest;
	float plrStomach;
	float plrLeg;
	float plrArm;

public:
// Monster Health & Damage
	float GetAGruntHealth() const
	{
		return agruntHealth;
	}

	float GetAGruntDmgPunch() const
	{
		return agruntDmgPunch;
	}

	float GetApacheHealth() const
	{
		return apacheHealth;
	}

	float GetBarneyHealth() const
	{
		return barneyHealth;
	}

	/**
	*	Multiply each node's health by this
	*/
	float GetBigMommaHealthFactor() const
	{
		return bigmommaHealthFactor;
	}

	/**
	*	melee attack damage
	*/
	float GetBigMommaDmgSlash() const
	{
		return bigmommaDmgSlash;
	}

	/**
	*	mortar attack damage
	*/
	float GetBigMommaDmgBlast() const
	{
		return bigmommaDmgBlast;
	}

	/**
	*	mortar attack radius
	*/
	float GetBigMommaRadiusBlast() const
	{
		return bigmommaRadiusBlast;
	}

	float GetBullsquidHealth() const
	{
		return bullsquidHealth;
	}

	float GetBullsquidDmgBite() const
	{
		return bullsquidDmgBite;
	}

	float GetBullsquidDmgWhip() const
	{
		return bullsquidDmgWhip;
	}

	float GetBullsquidDmgSpit() const
	{
		return bullsquidDmgSpit;
	}

	float GetGargantuaHealth() const
	{
		return gargantuaHealth;
	}

	float GetGargantuaDmgSlash() const
	{
		return gargantuaDmgSlash;
	}

	float GetGargantuaDmgFire() const
	{
		return gargantuaDmgFire;
	}

	float GetGargantuaDmgStomp() const
	{
		return gargantuaDmgStomp;
	}

	float GetHAssassinHealth() const
	{
		return hassassinHealth;
	}

	float GetHeadcrabHealth() const
	{
		return headcrabHealth;
	}

	float GetHeadcrabDmgBite() const
	{
		return headcrabDmgBite;
	}

	float GetHGruntHealth() const
	{
		return hgruntHealth;
	}

	float GetHGruntDmgKick() const
	{
		return hgruntDmgKick;
	}

	float GetHGruntShotgunPellets() const
	{
		return hgruntShotgunPellets;
	}

	float GetHGruntGrenadeSpeed() const
	{
		return hgruntGrenadeSpeed;
	}

	float GetHoundeyeHealth() const
	{
		return houndeyeHealth;
	}

	float GetHoundeyeDmgBlast() const
	{
		return houndeyeDmgBlast;
	}

	float GetSlaveHealth() const
	{
		return slaveHealth;
	}

	float GetSlaveDmgClaw() const
	{
		return slaveDmgClaw;
	}

	float GetSlaveDmgClawrake() const
	{
		return slaveDmgClawrake;
	}

	float GetSlaveDmgZap() const
	{
		return slaveDmgZap;
	}

	float GetIchthyosaurHealth() const
	{
		return ichthyosaurHealth;
	}

	float GetIchthyosaurDmgShake() const
	{
		return ichthyosaurDmgShake;
	}

	float GetLeechHealth() const
	{
		return leechHealth;
	}

	float GetLeechDmgBite() const
	{
		return leechDmgBite;
	}

	float GetControllerHealth() const
	{
		return controllerHealth;
	}

	float GetControllerDmgZap() const
	{
		return controllerDmgZap;
	}

	float GetControllerSpeedBall() const
	{
		return controllerSpeedBall;
	}

	float GetControllerDmgBall() const
	{
		return controllerDmgBall;
	}

	float GetNihilanthHealth() const
	{
		return nihilanthHealth;
	}

	float GetNihilanthZap() const
	{
		return nihilanthZap;
	}

	float GetScientistHealth() const
	{
		return scientistHealth;
	}

	float GetSnarkHealth() const
	{
		return snarkHealth;
	}

	float GetSnarkDmgBite() const
	{
		return snarkDmgBite;
	}

	float GetSnarkDmgPop() const
	{
		return snarkDmgPop;
	}

	float GetZombieHealth() const
	{
		return zombieHealth;
	}

	float GetZombieDmgOneSlash() const
	{
		return zombieDmgOneSlash;
	}

	float GetZombieDmgBothSlash() const
	{
		return zombieDmgBothSlash;
	}

	float GetTurretHealth() const
	{
		return turretHealth;
	}

	float GetMiniTurretHealth() const
	{
		return miniturretHealth;
	}

	float GetSentryHealth() const
	{
		return sentryHealth;
	}

// Player Weapons
	float GetPlrDmgCrowbar() const
	{
		return plrDmgCrowbar;
	}

	float GetDmg9MM() const
	{
		return plrDmg9MM;
	}

	float GetPlrDmg357() const
	{
		return plrDmg357;
	}

	float GetPlrDmgMP5() const
	{
		return plrDmgMP5;
	}

	float GetPlrDmgM203Grenade() const
	{
		return plrDmgM203Grenade;
	}

	float GetPlrDmgBuckshot() const
	{
		return plrDmgBuckshot;
	}

	float GetPlrDmgCrossbowClient() const
	{
		return plrDmgCrossbowClient;
	}

	float GetPlrDmgCrossbowMonster() const
	{
		return plrDmgCrossbowMonster;
	}

	float GetPlrDmgRPG() const
	{
		return plrDmgRPG;
	}

	float GetPlrDmgGauss() const
	{
		return plrDmgGauss;
	}

	float GetPlrDmgEgonNarrow() const
	{
		return plrDmgEgonNarrow;
	}

	float GetPlrDmgEgonWide() const
	{
		return plrDmgEgonWide;
	}

	float GetPlrDmgHornet() const
	{
		return plrDmgHornet;
	}

	float GetPlrDmgHandGrenade() const
	{
		return plrDmgHandGrenade;
	}

	float GetPlrDmgSatchel() const
	{
		return plrDmgSatchel;
	}

	float GetPlrDmgTripmine() const
	{
		return plrDmgTripmine;
	}

#if USE_OPFOR
	float GetPlrDmgKnife() const
	{
		return plrDmgKnife;
	}

	float GetPlrDmgPipewrench() const
	{
		return plrDmgPipewrench;
	}

	float GetPlrDmgGrapple() const
	{
		return plrDmgGrapple;
	}

	float GetPlrDmg556() const
	{
		return plrDmg556;
	}

	float GetPlrDmg762() const
	{
		return plrDmg762;
	}

	float GetPlrDmgDeagle() const
	{
		return plrDmgDeagle;
	}

	/**
	*	Singleplayer damage value.
	*/
	float GetPlrDmgShockRoachS() const
	{
		return plrDmgShockRoachS;
	}

	/**
	*	Multiplayer damage value.
	*/
	float GetPlrDmgShockRoachM() const
	{
		return plrDmgShockRoachM;
	}

	float GetPlrDmgDisplacerOther() const
	{
		return plrDmgDisplacerOther;
	}

	float GetPlrRadiusDisplacer() const
	{
		return plrRadiusDisplacer;
	}

	float GetPlrDmgSpore() const
	{
		return plrDmgSpore;
	}
#endif
	
// weapons shared by monsters
	float GetMonDmg9MM() const
	{
		return monDmg9MM;
	}

	float GetMonDmgMP5() const
	{
		return monDmgMP5;
	}

	float GetMonDmg12MM() const
	{
		return monDmg12MM;
	}

	float GetMonDmgHornet() const
	{
		return monDmgHornet;
	}

// health/suit charge
	float GetSuitChargerCapacity() const
	{
		return suitchargerCapacity;
	}

	float GetBatteryCapacity() const
	{
		return batteryCapacity;
	}

	float GetHealthChargerCapacity() const
	{
		return healthchargerCapacity;
	}

	float GetHealthKitCapacity() const
	{
		return healthkitCapacity;
	}

	float GetScientistHeal() const
	{
		return scientistHeal;
	}

// monster damage adj
	float GetMonHead() const
	{
		return monHead;
	}

	float GetMonChest() const
	{
		return monChest;
	}

	float GetMonStomach() const
	{
		return monStomach;
	}

	float GetMonLeg() const
	{
		return monLeg;
	}

	float GetMonArm() const
	{
		return monArm;
	}

// player damage adj
	float GetPlrHead() const
	{
		return plrHead;
	}

	float GetPlrChest() const
	{
		return plrChest;
	}

	float GetPlrStomach() const
	{
		return plrStomach;
	}

	float GetPlrLeg() const
	{
		return plrLeg;
	}

	float GetPlrArm() const
	{
		return plrArm;
	}
};

extern DLL_GLOBAL skilldata_t gSkillData;

#endif //GAME_SERVER_SKILL_H