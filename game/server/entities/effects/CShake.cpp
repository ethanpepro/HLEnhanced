#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CShake.h"

LINK_ENTITY_TO_CLASS( env_shake, CShake );

void CShake::Spawn( void )
{
	pev->solid = SOLID_NOT;
	pev->movetype = MOVETYPE_NONE;
	pev->effects = 0;
	pev->frame = 0;

	if( pev->spawnflags & SF_SHAKE_EVERYONE )
		pev->dmg = 0;
}

void CShake::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	UTIL_ScreenShake( GetAbsOrigin(), Amplitude(), Frequency(), Duration(), Radius() );
}

void CShake::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "amplitude" ) )
	{
		SetAmplitude( atof( pkvd->szValue ) );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "frequency" ) )
	{
		SetFrequency( atof( pkvd->szValue ) );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "duration" ) )
	{
		SetDuration( atof( pkvd->szValue ) );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "radius" ) )
	{
		SetRadius( atof( pkvd->szValue ) );
		pkvd->fHandled = true;
	}
	else
		CPointEntity::KeyValue( pkvd );
}