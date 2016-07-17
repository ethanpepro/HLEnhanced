#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CFuncTrackTrain.h"

#include "CFuncTrainControls.h"

LINK_ENTITY_TO_CLASS( func_traincontrols, CFuncTrainControls );

void CFuncTrainControls::Spawn( void )
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	SET_MODEL( ENT( pev ), STRING( pev->model ) );

	UTIL_SetSize( this, pev->mins, pev->maxs );
	UTIL_SetOrigin( this, pev->origin );

	SetThink( &CFuncTrainControls::Find );
	pev->nextthink = gpGlobals->time;
}

void CFuncTrainControls::Find( void )
{
	edict_t *pTarget = NULL;

	do
	{
		pTarget = FIND_ENTITY_BY_TARGETNAME( pTarget, STRING( pev->target ) );
	}
	while( !FNullEnt( pTarget ) && !FClassnameIs( pTarget, "func_tracktrain" ) );

	if( FNullEnt( pTarget ) )
	{
		ALERT( at_console, "No train %s\n", STRING( pev->target ) );
		return;
	}

	CFuncTrackTrain *ptrain = CFuncTrackTrain::Instance( pTarget );
	ptrain->SetControls( this );
	UTIL_Remove( this );
}