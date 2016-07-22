#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CTriggerCDAudio.h"

#include "CTargetCDAudio.h"

LINK_ENTITY_TO_CLASS( target_cdaudio, CTargetCDAudio );

void CTargetCDAudio::Spawn( void )
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;

	if( pev->scale > 0 )
		pev->nextthink = gpGlobals->time + 1.0;
}

void CTargetCDAudio::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "radius" ) )
	{
		pev->scale = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CPointEntity::KeyValue( pkvd );
}

void CTargetCDAudio::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	Play();
}

// only plays for ONE client, so only use in single play!
void CTargetCDAudio::Think( void )
{
	edict_t *pClient;

	// manually find the single player. 
	pClient = g_engfuncs.pfnPEntityOfEntIndex( 1 );

	// Can't play if the client is not connected!
	if( !pClient )
		return;

	pev->nextthink = gpGlobals->time + 0.5;

	if( ( pClient->v.origin - GetAbsOrigin() ).Length() <= pev->scale )
		Play();

}

void CTargetCDAudio::Play( void )
{
	PlayCDTrack( ( int ) pev->health );
	UTIL_Remove( this );
}