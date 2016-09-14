#include "extdll.h"
#include "util.h"
#include "gamerules/GameRules.h"
#include "cbase.h"

#include "CTriggerEndSection.h"

BEGIN_DATADESC( CTriggerEndSection )
	DEFINE_TOUCHFUNC( EndSectionTouch ),
	DEFINE_USEFUNC( EndSectionUse ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_endsection, CTriggerEndSection );

void CTriggerEndSection::Spawn( void )
{
	if( g_pGameRules->IsDeathmatch() )
	{
		UTIL_RemoveNow( this );
		return;
	}

	InitTrigger();

	SetUse( &CTriggerEndSection::EndSectionUse );
	// If it is a "use only" trigger, then don't set the touch function.
	if( !( pev->spawnflags & SF_ENDSECTION_USEONLY ) )
		SetTouch( &CTriggerEndSection::EndSectionTouch );
}

void CTriggerEndSection::EndSectionTouch( CBaseEntity *pOther )
{
	// Only save on clients
	if( !pOther->IsNetClient() )
		return;

	SetTouch( NULL );

	if( pev->message )
	{
		g_engfuncs.pfnEndSection( STRING( pev->message ) );
	}
	UTIL_Remove( this );
}

void CTriggerEndSection::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "section" ) )
	{
		//		m_iszSectionName = ALLOC_STRING( pkvd->szValue );
		// Store this in message so we don't have to write save/restore for this ent
		pev->message = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseTrigger::KeyValue( pkvd );
}

void CTriggerEndSection::EndSectionUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	// Only save on clients
	if( pActivator && !pActivator->IsNetClient() )
		return;

	SetUse( NULL );

	if( pev->message )
	{
		g_engfuncs.pfnEndSection( STRING( pev->message ) );
	}
	UTIL_Remove( this );
}