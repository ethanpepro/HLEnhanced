#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CEnvSpark.h"

//TODO: move to some other header - Solokiller
void DoSpark( entvars_t *pev, const Vector &location );

BEGIN_DATADESC( CEnvSpark )
	DEFINE_FIELD( m_flDelay, FIELD_FLOAT ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( env_spark, CEnvSpark );
LINK_ENTITY_TO_CLASS( env_debris, CEnvSpark );

void CEnvSpark::Spawn( void )
{
	SetThink( NULL );
	SetUse( NULL );

	//TODO: define spawn flags - Solokliler
	if( FBitSet( pev->spawnflags, 32 ) ) // Use for on/off
	{
		if( FBitSet( pev->spawnflags, 64 ) ) // Start on
		{
			SetThink( &CEnvSpark::SparkThink );	// start sparking
			SetUse( &CEnvSpark::SparkStop );		// set up +USE to stop sparking
		}
		else
			SetUse( &CEnvSpark::SparkStart );
	}
	else
		SetThink( &CEnvSpark::SparkThink );

	pev->nextthink = gpGlobals->time + ( 0.1 + RANDOM_FLOAT( 0, 1.5 ) );

	if( m_flDelay <= 0 )
		m_flDelay = 1.5;

	Precache();
}

void CEnvSpark::Precache( void )
{
	PRECACHE_SOUND( "buttons/spark1.wav" );
	PRECACHE_SOUND( "buttons/spark2.wav" );
	PRECACHE_SOUND( "buttons/spark3.wav" );
	PRECACHE_SOUND( "buttons/spark4.wav" );
	PRECACHE_SOUND( "buttons/spark5.wav" );
	PRECACHE_SOUND( "buttons/spark6.wav" );
}

void EXPORT CEnvSpark::SparkThink( void )
{
	pev->nextthink = gpGlobals->time + 0.1 + RANDOM_FLOAT( 0, m_flDelay );
	DoSpark( pev, pev->origin );
}

void EXPORT CEnvSpark::SparkStart( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	SetUse( &CEnvSpark::SparkStop );
	SetThink( &CEnvSpark::SparkThink );
	pev->nextthink = gpGlobals->time + ( 0.1 + RANDOM_FLOAT( 0, m_flDelay ) );
}

void EXPORT CEnvSpark::SparkStop( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	SetUse( &CEnvSpark::SparkStart );
	SetThink( NULL );
}

void CEnvSpark::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "MaxDelay" ) )
	{
		m_flDelay = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "style" ) ||
			 FStrEq( pkvd->szKeyName, "height" ) ||
			 FStrEq( pkvd->szKeyName, "killtarget" ) ||
			 FStrEq( pkvd->szKeyName, "value1" ) ||
			 FStrEq( pkvd->szKeyName, "value2" ) ||
			 FStrEq( pkvd->szKeyName, "value3" ) )
		pkvd->fHandled = true;
	else
		CBaseEntity::KeyValue( pkvd );
}