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
#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CMultiDamage.h"

void CMultiDamage::AddMultiDamage( entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType )
{
	if( !pEntity )
		return;

	m_bitsDamageTypes |= bitsDamageType;

	if( pEntity != m_pEntity )
	{
		//TODO: replace this - Solokiller
		ApplyMultiDamage( CBaseEntity::Instance( pevInflictor ), CBaseEntity::Instance( pevInflictor ) ); // UNDONE: wrong attacker!
		m_pEntity = pEntity;
		m_flAmount = 0;
	}

	m_flAmount += flDamage;
}

void CMultiDamage::ApplyMultiDamage( CBaseEntity* pInflictor, CBaseEntity* pAttacker )
{
	if( !m_pEntity )
		return;

	m_pEntity->TakeDamage( pInflictor, pAttacker, m_flAmount, m_bitsDamageTypes );
}