#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CInfoIntermission.h"

LINK_ENTITY_TO_CLASS( info_intermission, CInfoIntermission );

void CInfoIntermission::Spawn( void )
{
	SetAbsOrigin( GetAbsOrigin() );
	pev->solid = SOLID_NOT;
	pev->effects = EF_NODRAW;
	pev->v_angle = g_vecZero;

	pev->nextthink = gpGlobals->time + 2;// let targets spawn!

}

void CInfoIntermission::Think( void )
{
	// find my target
	CBaseEntity* pTarget = UTIL_FindEntityByTargetname( nullptr, GetTarget() );

	if( pTarget )
	{
		pev->v_angle = UTIL_VecToAngles( ( pTarget->GetAbsOrigin() - GetAbsOrigin() ).Normalize() );
		pev->v_angle.x = -pev->v_angle.x;
	}
}