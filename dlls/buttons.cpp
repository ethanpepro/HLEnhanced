/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
/*

===== buttons.cpp ========================================================

  button-related code

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "saverestore.h"
#include "doors.h"

#if !defined ( _WIN32 )
#include <string.h> // memset())))
#endif

#define SF_GLOBAL_SET			1	// Set global state to initial state on spawn

class CEnvGlobal : public CPointEntity
{
public:
	DECLARE_CLASS( CEnvGlobal, CPointEntity );
	DECLARE_DATADESC();

	void	Spawn( void ) override;
	void	KeyValue( KeyValueData *pkvd ) override;
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	
	string_t	m_globalstate;
	int			m_triggermode;
	int			m_initialstate;
};

BEGIN_DATADESC( CEnvGlobal )
	DEFINE_FIELD( m_globalstate, FIELD_STRING ),
	DEFINE_FIELD( m_triggermode, FIELD_INTEGER ),
	DEFINE_FIELD( m_initialstate, FIELD_INTEGER ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( env_global, CEnvGlobal );

void CEnvGlobal::KeyValue( KeyValueData *pkvd )
{
	pkvd->fHandled = true;

	if ( FStrEq(pkvd->szKeyName, "globalstate") )		// State name
		m_globalstate = ALLOC_STRING( pkvd->szValue );
	else if ( FStrEq(pkvd->szKeyName, "triggermode") )
		m_triggermode = atoi( pkvd->szValue );
	else if ( FStrEq(pkvd->szKeyName, "initialstate") )
		m_initialstate = atoi( pkvd->szValue );
	else 
		CPointEntity::KeyValue( pkvd );
}

void CEnvGlobal::Spawn( void )
{
	if ( !m_globalstate )
	{
		REMOVE_ENTITY( ENT(pev) );
		return;
	}
	if ( FBitSet( pev->spawnflags, SF_GLOBAL_SET ) )
	{
		if ( !gGlobalState.EntityInTable( m_globalstate ) )
			gGlobalState.EntityAdd( m_globalstate, gpGlobals->mapname, (GLOBALESTATE)m_initialstate );
	}
}


void CEnvGlobal::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	GLOBALESTATE oldState = gGlobalState.EntityGetState( m_globalstate );
	GLOBALESTATE newState;

	switch( m_triggermode )
	{
	case 0:
		newState = GLOBAL_OFF;
		break;

	case 1:
		newState = GLOBAL_ON;
		break;

	case 2:
		newState = GLOBAL_DEAD;
		break;

	default:
	case 3:
		if ( oldState == GLOBAL_ON )
			newState = GLOBAL_OFF;
		else if ( oldState == GLOBAL_OFF )
			newState = GLOBAL_ON;
		else
			newState = oldState;
	}

	if ( gGlobalState.EntityInTable( m_globalstate ) )
		gGlobalState.EntitySetState( m_globalstate, newState );
	else
		gGlobalState.EntityAdd( m_globalstate, gpGlobals->mapname, newState );
}

//TODO: move to lock sound file - Solokiller
// Button sound table. 
// Also used by CBaseDoor to get 'touched' door lock/unlock sounds

char *ButtonSound( int sound )
{ 
	char *pszSound;

	switch ( sound )
	{
		case 0: pszSound = "common/null.wav";        break;
		case 1: pszSound = "buttons/button1.wav";	break;
		case 2: pszSound = "buttons/button2.wav";	break;
		case 3: pszSound = "buttons/button3.wav";	break;
		case 4: pszSound = "buttons/button4.wav";	break;
		case 5: pszSound = "buttons/button5.wav";	break;
		case 6: pszSound = "buttons/button6.wav";	break;
		case 7: pszSound = "buttons/button7.wav";	break;
		case 8: pszSound = "buttons/button8.wav";	break;
		case 9: pszSound = "buttons/button9.wav";	break;
		case 10: pszSound = "buttons/button10.wav";	break;
		case 11: pszSound = "buttons/button11.wav";	break;
		case 12: pszSound = "buttons/latchlocked1.wav";	break;
		case 13: pszSound = "buttons/latchunlocked1.wav";	break;
		case 14: pszSound = "buttons/lightswitch2.wav";break;

// next 6 slots reserved for any additional sliding button sounds we may add
		
		case 21: pszSound = "buttons/lever1.wav";	break;
		case 22: pszSound = "buttons/lever2.wav";	break;
		case 23: pszSound = "buttons/lever3.wav";	break;
		case 24: pszSound = "buttons/lever4.wav";	break;
		case 25: pszSound = "buttons/lever5.wav";	break;

		default:pszSound = "buttons/button9.wav";	break;
	}

	return pszSound;
}

//----------------------------------------------------------------
// Spark
//----------------------------------------------------------------

class CEnvSpark : public CBaseEntity
{
public:
	DECLARE_CLASS( CEnvSpark, CBaseEntity );
	DECLARE_DATADESC();

	void	Spawn(void) override;
	void	Precache(void) override;
	void	EXPORT SparkThink(void);
	void	EXPORT SparkStart(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	EXPORT SparkStop(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	KeyValue(KeyValueData *pkvd) override;

	float	m_flDelay;
};


BEGIN_DATADESC( CEnvSpark )
	DEFINE_FIELD( m_flDelay, FIELD_FLOAT),
END_DATADESC()

LINK_ENTITY_TO_CLASS(env_spark, CEnvSpark);
LINK_ENTITY_TO_CLASS(env_debris, CEnvSpark);

void CEnvSpark::Spawn(void)
{
	SetThink( NULL );
	SetUse( NULL );

	//TODO: define spawn flags - Solokliler
	if (FBitSet(pev->spawnflags, 32)) // Use for on/off
	{
		if (FBitSet(pev->spawnflags, 64)) // Start on
		{
			SetThink(&CEnvSpark::SparkThink);	// start sparking
			SetUse(&CEnvSpark::SparkStop);		// set up +USE to stop sparking
		}
		else
			SetUse(&CEnvSpark::SparkStart);
	}
	else
		SetThink(&CEnvSpark::SparkThink);
		
	pev->nextthink = gpGlobals->time + ( 0.1 + RANDOM_FLOAT ( 0, 1.5 ) );

	if (m_flDelay <= 0)
		m_flDelay = 1.5;

	Precache( );
}


void CEnvSpark::Precache(void)
{
	PRECACHE_SOUND( "buttons/spark1.wav" );
	PRECACHE_SOUND( "buttons/spark2.wav" );
	PRECACHE_SOUND( "buttons/spark3.wav" );
	PRECACHE_SOUND( "buttons/spark4.wav" );
	PRECACHE_SOUND( "buttons/spark5.wav" );
	PRECACHE_SOUND( "buttons/spark6.wav" );
}

void CEnvSpark::KeyValue( KeyValueData *pkvd )
{
	if (FStrEq(pkvd->szKeyName, "MaxDelay"))
	{
		m_flDelay = atof(pkvd->szValue);
		pkvd->fHandled = true;	
	}
	else if (	FStrEq(pkvd->szKeyName, "style") ||
				FStrEq(pkvd->szKeyName, "height") ||
				FStrEq(pkvd->szKeyName, "killtarget") ||
				FStrEq(pkvd->szKeyName, "value1") ||
				FStrEq(pkvd->szKeyName, "value2") ||
				FStrEq(pkvd->szKeyName, "value3"))
		pkvd->fHandled = true;
	else
		CBaseEntity::KeyValue( pkvd );
}

//TODO: move to some other header - Solokiller
void DoSpark( entvars_t *pev, const Vector &location );

void EXPORT CEnvSpark::SparkThink(void)
{
	pev->nextthink = gpGlobals->time + 0.1 + RANDOM_FLOAT (0, m_flDelay);
	DoSpark( pev, pev->origin );
}

void EXPORT CEnvSpark::SparkStart(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	SetUse(&CEnvSpark::SparkStop);
	SetThink(&CEnvSpark::SparkThink);
	pev->nextthink = gpGlobals->time + (0.1 + RANDOM_FLOAT ( 0, m_flDelay));
}

void EXPORT CEnvSpark::SparkStop(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	SetUse(&CEnvSpark::SparkStart);
	SetThink(NULL);
}

#define SF_BTARGET_USE		0x0001
#define SF_BTARGET_ON		0x0002

class CButtonTarget : public CBaseEntity
{
public:
	DECLARE_CLASS( CButtonTarget, CBaseEntity );

	void Spawn( void ) override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	int TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType ) override;
	int	ObjectCaps( void ) override;
	
};

LINK_ENTITY_TO_CLASS( button_target, CButtonTarget );

void CButtonTarget::Spawn( void )
{
	pev->movetype	= MOVETYPE_PUSH;
	pev->solid		= SOLID_BSP;
	SET_MODEL(ENT(pev), STRING(pev->model));
	pev->takedamage = DAMAGE_YES;

	if ( FBitSet( pev->spawnflags, SF_BTARGET_ON ) )
		pev->frame = 1;
}

void CButtonTarget::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( !ShouldToggle( useType, static_cast<int>( pev->frame ) != 0 ) )
		return;
	pev->frame = 1-pev->frame;
	if ( pev->frame )
		SUB_UseTargets( pActivator, USE_ON, 0 );
	else
		SUB_UseTargets( pActivator, USE_OFF, 0 );
}


int	CButtonTarget :: ObjectCaps( void )
{
	int caps = CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION;

	if ( FBitSet(pev->spawnflags, SF_BTARGET_USE) )
		return caps | FCAP_IMPULSE_USE;
	else
		return caps;
}


int CButtonTarget::TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType )
{
	Use( Instance(pevAttacker), this, USE_TOGGLE, 0 );

	return 1;
}
