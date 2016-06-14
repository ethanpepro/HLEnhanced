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
		ApplyMultiDamage( pevInflictor, pevInflictor ); // UNDONE: wrong attacker!
		m_pEntity = pEntity;
		m_flAmount = 0;
	}

	m_flAmount += flDamage;
}

void CMultiDamage::ApplyMultiDamage( entvars_t *pevInflictor, entvars_t *pevAttacker )
{
	if( !m_pEntity )
		return;

	m_pEntity->TakeDamage( pevInflictor, pevAttacker, m_flAmount, m_bitsDamageTypes );
}