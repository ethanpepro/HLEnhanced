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
#ifndef GAME_SERVER_CMULTIDAMAGE_H
#define GAME_SERVER_CMULTIDAMAGE_H

typedef struct entvars_s entvars_t;

class CBaseEntity;

/**
*	MULTI - DAMAGE
*
*	Collects multiple small damages into a single damage
*/
class CMultiDamage final
{
public:
	/**
	*	Constructor.
	*/
	CMultiDamage()
	{
		Clear();
	}

	/**
	*	@return The entity currently being attacked.
	*/
	CBaseEntity* GetEntity() { return m_pEntity; }

	/**
	*	@return The total amount of damage to deal so far.
	*/
	float GetAmount() const { return m_flAmount; }

	/**
	*	@return Bit vector of damage types to deal so far.
	*/
	int GetDamageTypes() const { return m_bitsDamageTypes; }

	/**
	*	Sets the damage types.
	*/
	void SetDamageTypes( const int bitsDamageTypes )
	{
		m_bitsDamageTypes = bitsDamageTypes;
	}

	/**
	*	Resets the multi damage accumulator.
	*/
	void Clear()
	{
		m_pEntity = nullptr;
		m_flAmount = 0;
		m_bitsDamageTypes = 0;
	}

	/**
	*	Adds multi-damage to deal to the given entity. Damage will be accumulated, but not applied.
	*	@param pevInflictor Entity that's dealing damage.
	*	@param pEntity Entity that's being attacked.
	*	@param flDamage Damage amount to add.
	*	@param bitsDamageType Bit vector of damage types to add.
	*/
	void AddMultiDamage( entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType );

	/**
	*	Applies multi-damage to the target.
	*	@param pevInflictor Inflictor to pass to TakeDamage.
	*	@param pevAttacker Attacker to pass to TakeDamage.
	*/
	void ApplyMultiDamage( entvars_t *pevInflictor, entvars_t *pevAttacker );

private:
	CBaseEntity* m_pEntity;
	float m_flAmount;
	int m_bitsDamageTypes;
};

#endif //GAME_SERVER_CMULTIDAMAGE_H