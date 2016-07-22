#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerMonsterJump.h"

LINK_ENTITY_TO_CLASS( trigger_monsterjump, CTriggerMonsterJump );

void CTriggerMonsterJump::Spawn( void )
{
	SetMovedir( this );

	InitTrigger();

	pev->nextthink = 0;
	pev->speed = 200;
	m_flHeight = 150;

	if( HasTargetname() )
	{// if targetted, spawn turned off
		pev->solid = SOLID_NOT;
		SetAbsOrigin( GetAbsOrigin() ); // Unlink from trigger list
		SetUse( &CTriggerMonsterJump::ToggleUse );
	}
}

void CTriggerMonsterJump::Think( void )
{
	pev->solid = SOLID_NOT;// kill the trigger for now !!!UNDONE
	SetAbsOrigin( GetAbsOrigin() ); // Unlink from trigger list
	SetThink( NULL );
}

void CTriggerMonsterJump::Touch( CBaseEntity *pOther )
{
	if( !FBitSet( pOther->pev->flags, FL_MONSTER ) )
	{// touched by a non-monster.
		return;
	}

	pOther->pev->origin.z += 1;

	if( FBitSet( pOther->pev->flags, FL_ONGROUND ) )
	{
		// clear the onground so physics don't bitch
		pOther->pev->flags &= ~FL_ONGROUND;
	}

	// toss the monster!
	pOther->pev->velocity = pev->movedir * pev->speed;
	pOther->pev->velocity.z += m_flHeight;
	pev->nextthink = gpGlobals->time;
}
