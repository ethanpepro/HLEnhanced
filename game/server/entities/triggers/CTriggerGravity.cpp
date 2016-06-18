#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerGravity.h"

LINK_ENTITY_TO_CLASS( trigger_gravity, CTriggerGravity );

void CTriggerGravity::Spawn( void )
{
	InitTrigger();
	SetTouch( &CTriggerGravity::GravityTouch );
}

void CTriggerGravity::GravityTouch( CBaseEntity *pOther )
{
	// Only save on clients
	if( !pOther->IsPlayer() )
		return;

	pOther->pev->gravity = pev->gravity;
}