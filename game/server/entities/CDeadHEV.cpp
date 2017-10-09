#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CDeadHEV.h"

const char* const CDeadHEV::m_szPoses[] = { "deadback", "deadsitting", "deadstomach", "deadtable" };

LINK_ENTITY_TO_CLASS( monster_hevsuit_dead, CDeadHEV );

//=========================================================
// ********** DeadHEV SPAWN **********
//=========================================================
void CDeadHEV::Spawn( void )
{
	PRECACHE_MODEL( "models/player.mdl" );
	SetModel( "models/player.mdl" );

	GetEffects().ClearAll();
	SetYawSpeed( 8 );
	pev->sequence = 0;
	pev->body = 1;
	m_bloodColor = BLOOD_COLOR_RED;

	pev->sequence = LookupSequence( m_szPoses[ m_iPose ] );

	if( pev->sequence == -1 )
	{
		ALERT( at_console, "Dead hevsuit with bad pose\n" );
		pev->sequence = 0;
		GetEffects() = EF_BRIGHTFIELD;
	}

	// Corpses have less health
	SetHealth( 8 );

	MonsterInitDead();
}

void CDeadHEV::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "pose" ) )
	{
		m_iPose = atoi( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseMonster::KeyValue( pkvd );
}