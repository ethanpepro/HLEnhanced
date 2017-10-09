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
	SetSpeed( 200 );
	m_flHeight = 150;

	if( HasTargetname() )
	{// if targetted, spawn turned off
		SetSolidType( SOLID_NOT );
		SetAbsOrigin( GetAbsOrigin() ); // Unlink from trigger list
		SetUse( &CTriggerMonsterJump::ToggleUse );
	}
}

void CTriggerMonsterJump::Think( void )
{
	SetSolidType( SOLID_NOT );// kill the trigger for now !!!UNDONE
	SetAbsOrigin( GetAbsOrigin() ); // Unlink from trigger list
	SetThink( NULL );
}

void CTriggerMonsterJump::Touch( CBaseEntity *pOther )
{
	if( !FBitSet( pOther->pev->flags, FL_MONSTER ) )
	{// touched by a non-monster.
		return;
	}

	Vector vecOrigin = pOther->GetAbsOrigin();
	vecOrigin.z += 1;
	pOther->SetAbsOrigin( vecOrigin );

	if( FBitSet( pOther->pev->flags, FL_ONGROUND ) )
	{
		// clear the onground so physics don't bitch
		pOther->pev->flags &= ~FL_ONGROUND;
	}

	// toss the monster!
	Vector vecVelocity = pev->movedir * GetSpeed();
	vecVelocity.z += m_flHeight;
	pOther->SetAbsVelocity( vecVelocity );
	pev->nextthink = gpGlobals->time;
}
