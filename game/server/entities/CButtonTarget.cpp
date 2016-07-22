#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CButtonTarget.h"

LINK_ENTITY_TO_CLASS( button_target, CButtonTarget );

void CButtonTarget::Spawn( void )
{
	pev->movetype = MOVETYPE_PUSH;
	pev->solid = SOLID_BSP;
	SetModel( STRING( pev->model ) );
	pev->takedamage = DAMAGE_YES;

	if( FBitSet( pev->spawnflags, SF_BTARGET_ON ) )
		pev->frame = 1;
}

void CButtonTarget::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( !ShouldToggle( useType, static_cast<int>( pev->frame ) != 0 ) )
		return;
	pev->frame = 1 - pev->frame;
	if( pev->frame )
		SUB_UseTargets( pActivator, USE_ON, 0 );
	else
		SUB_UseTargets( pActivator, USE_OFF, 0 );
}

void CButtonTarget::OnTakeDamage( const CTakeDamageInfo& info )
{
	Use( info.GetAttacker(), this, USE_TOGGLE, 0 );
}

int	CButtonTarget::ObjectCaps() const
{
	int caps = CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION;

	if( FBitSet( pev->spawnflags, SF_BTARGET_USE ) )
		return caps | FCAP_IMPULSE_USE;
	else
		return caps;
}