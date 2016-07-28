#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerVolume.h"

LINK_ENTITY_TO_CLASS( trigger_transition, CTriggerVolume );

void CTriggerVolume::Spawn( void )
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	SetModel( STRING( pev->model ) );    // set size and link into world
	pev->model = iStringNull;
	pev->modelindex = 0;
}