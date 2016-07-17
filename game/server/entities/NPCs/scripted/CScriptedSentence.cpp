#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CScriptedSentence.h"

BEGIN_DATADESC( CScriptedSentence )
	DEFINE_FIELD( m_iszSentence, FIELD_STRING ),
	DEFINE_FIELD( m_iszEntity, FIELD_STRING ),
	DEFINE_FIELD( m_flRadius, FIELD_FLOAT ),
	DEFINE_FIELD( m_flDuration, FIELD_FLOAT ),
	DEFINE_FIELD( m_flRepeat, FIELD_FLOAT ),
	DEFINE_FIELD( m_flAttenuation, FIELD_FLOAT ),
	DEFINE_FIELD( m_flVolume, FIELD_FLOAT ),
	DEFINE_FIELD( m_active, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_iszListener, FIELD_STRING ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( scripted_sentence, CScriptedSentence );

void CScriptedSentence::Spawn( void )
{
	pev->solid = SOLID_NOT;

	m_active = true;
	// if no targetname, start now
	if( !pev->targetname )
	{
		SetThink( &CScriptedSentence::FindThink );
		pev->nextthink = gpGlobals->time + 1.0;
	}

	switch( static_cast<SoundRadius>( pev->impulse ) )
	{
	case SoundRadius::MEDIUM:			// Medium radius
		m_flAttenuation = ATTN_STATIC;
		break;

	case SoundRadius::LARGE:			// Large radius
		m_flAttenuation = ATTN_NORM;
		break;

	case SoundRadius::PLAY_EVERYWHERE:	//EVERYWHERE
		m_flAttenuation = ATTN_NONE;
		break;

	default:
	case SoundRadius::SMALL:			// Small radius
		m_flAttenuation = ATTN_IDLE;
		break;
	}
	pev->impulse = 0;

	// No volume, use normal
	if( m_flVolume <= 0 )
		m_flVolume = 1.0;
}

void CScriptedSentence::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "sentence" ) )
	{
		m_iszSentence = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "entity" ) )
	{
		m_iszEntity = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "duration" ) )
	{
		m_flDuration = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "radius" ) )
	{
		m_flRadius = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "refire" ) )
	{
		m_flRepeat = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "attenuation" ) )
	{
		pev->impulse = atoi( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "volume" ) )
	{
		m_flVolume = atof( pkvd->szValue ) * 0.1;
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "listener" ) )
	{
		m_iszListener = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseToggle::KeyValue( pkvd );
}

void CScriptedSentence::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( !m_active )
		return;
	//	ALERT( at_console, "Firing sentence: %s\n", STRING(m_iszSentence) );
	SetThink( &CScriptedSentence::FindThink );
	pev->nextthink = gpGlobals->time;
}

void CScriptedSentence::FindThink( void )
{
	CBaseMonster *pMonster = FindEntity();
	if( pMonster )
	{
		StartSentence( pMonster );
		if( pev->spawnflags & SF_SENTENCE_ONCE )
			UTIL_Remove( this );
		SetThink( &CScriptedSentence::DelayThink );
		pev->nextthink = gpGlobals->time + m_flDuration + m_flRepeat;
		m_active = false;
		//		ALERT( at_console, "%s: found monster %s\n", STRING(m_iszSentence), STRING(m_iszEntity) );
	}
	else
	{
		//		ALERT( at_console, "%s: can't find monster %s\n", STRING(m_iszSentence), STRING(m_iszEntity) );
		pev->nextthink = gpGlobals->time + m_flRepeat + 0.5;
	}
}

void CScriptedSentence::DelayThink( void )
{
	m_active = true;
	if( !pev->targetname )
		pev->nextthink = gpGlobals->time + 0.1;
	SetThink( &CScriptedSentence::FindThink );
}

CBaseMonster *CScriptedSentence::FindEntity( void )
{
	edict_t *pentTarget;
	CBaseMonster *pMonster;


	pentTarget = FIND_ENTITY_BY_TARGETNAME( NULL, STRING( m_iszEntity ) );
	pMonster = NULL;

	while( !FNullEnt( pentTarget ) )
	{
		pMonster = GetMonsterPointer( pentTarget );
		if( pMonster != NULL )
		{
			if( AcceptableSpeaker( pMonster ) )
				return pMonster;
			//			ALERT( at_console, "%s (%s), not acceptable\n", STRING(pMonster->pev->classname), STRING(pMonster->pev->targetname) );
		}
		pentTarget = FIND_ENTITY_BY_TARGETNAME( pentTarget, STRING( m_iszEntity ) );
	}

	CBaseEntity *pEntity = NULL;
	while( ( pEntity = UTIL_FindEntityInSphere( pEntity, pev->origin, m_flRadius ) ) != NULL )
	{
		if( FClassnameIs( pEntity->pev, STRING( m_iszEntity ) ) )
		{
			if( FBitSet( pEntity->pev->flags, FL_MONSTER ) )
			{
				pMonster = pEntity->MyMonsterPointer();
				if( AcceptableSpeaker( pMonster ) )
					return pMonster;
			}
		}
	}

	return NULL;
}

bool CScriptedSentence::AcceptableSpeaker( const CBaseMonster *pMonster ) const
{
	if( pMonster )
	{
		if( pev->spawnflags & SF_SENTENCE_FOLLOWERS )
		{
			if( pMonster->m_hTargetEnt == NULL || !FClassnameIs( pMonster->m_hTargetEnt->pev, "player" ) )
				return false;
		}
		bool override;
		if( pev->spawnflags & SF_SENTENCE_INTERRUPT )
			override = true;
		else
			override = false;
		if( pMonster->CanPlaySentence( override ) )
			return true;
	}
	return false;
}

bool CScriptedSentence::StartSentence( CBaseMonster *pTarget )
{
	if( !pTarget )
	{
		ALERT( at_aiconsole, "Not Playing sentence %s\n", STRING( m_iszSentence ) );
		return false;
	}

	bool bConcurrent = false;
	if( !( pev->spawnflags & SF_SENTENCE_CONCURRENT ) )
		bConcurrent = true;

	CBaseEntity *pListener = NULL;
	if( !FStringNull( m_iszListener ) )
	{
		float radius = m_flRadius;

		if( FStrEq( STRING( m_iszListener ), "player" ) )
			radius = 4096;	// Always find the player

		pListener = UTIL_FindEntityGeneric( STRING( m_iszListener ), pTarget->pev->origin, radius );
	}

	pTarget->PlayScriptedSentence( STRING( m_iszSentence ), m_flDuration, m_flVolume, m_flAttenuation, bConcurrent, pListener );
	ALERT( at_aiconsole, "Playing sentence %s (%.1f)\n", STRING( m_iszSentence ), m_flDuration );
	SUB_UseTargets( NULL, USE_TOGGLE, 0 );
	return true;
}