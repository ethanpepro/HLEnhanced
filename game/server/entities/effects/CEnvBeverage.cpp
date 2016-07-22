#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CEnvBeverage.h"

LINK_ENTITY_TO_CLASS( env_beverage, CEnvBeverage );

void CEnvBeverage::Spawn( void )
{
	Precache();
	pev->solid = SOLID_NOT;
	pev->effects = EF_NODRAW;
	pev->frags = 0;

	if( pev->health == 0 )
	{
		pev->health = 10;
	}
}

void CEnvBeverage::Precache( void )
{
	PRECACHE_MODEL( "models/can.mdl" );
	PRECACHE_SOUND( "weapons/g_bounce3.wav" );
}

void CEnvBeverage::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( pev->frags != 0 || pev->health <= 0 )
	{
		// no more cans while one is waiting in the dispenser, or if I'm out of cans.
		return;
	}

	CBaseEntity *pCan = CBaseEntity::Create( "item_sodacan", GetAbsOrigin(), pev->angles, edict() );

	if( pev->skin == 6 )
	{
		// random
		pCan->pev->skin = RANDOM_LONG( 0, 5 );
	}
	else
	{
		pCan->pev->skin = pev->skin;
	}

	pev->frags = 1;
	pev->health--;

	//SetThink (SUB_Remove);
	//pev->nextthink = gpGlobals->time;
}