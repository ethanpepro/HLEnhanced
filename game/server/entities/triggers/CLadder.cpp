#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CLadder.h"

LINK_ENTITY_TO_CLASS( func_ladder, CLadder );

void CLadder::KeyValue( KeyValueData *pkvd )
{
	CBaseTrigger::KeyValue( pkvd );
}

void CLadder::Spawn( void )
{
	Precache();

	SetModel( STRING( pev->model ) );    // set size and link into world
	pev->movetype = MOVETYPE_PUSH;
}

void CLadder::Precache( void )
{
	// Do all of this in here because we need to 'convert' old saved games
	pev->solid = SOLID_NOT;
	pev->skin = CONTENTS_LADDER;
	if( CVAR_GET_FLOAT( "showtriggers" ) == 0 )
	{
		pev->rendermode = kRenderTransTexture;
		pev->renderamt = 0;
	}
	pev->effects &= ~EF_NODRAW;
}