#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CBeam.h"
#include "CSprite.h"

#include "CLaser.h"

BEGIN_DATADESC( CLaser )
	DEFINE_FIELD( m_pSprite, FIELD_CLASSPTR ),
	DEFINE_FIELD( m_iszSpriteName, FIELD_STRING ),
	DEFINE_FIELD( m_firePosition, FIELD_POSITION_VECTOR ),
	DEFINE_THINKFUNC( StrikeThink ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( env_laser, CLaser );

void CLaser::Spawn( void )
{
	if( FStringNull( pev->model ) )
	{
		SetThink( &CLaser::SUB_Remove );
		return;
	}
	pev->solid = SOLID_NOT;							// Remove model & collisions
	Precache();

	SetThink( &CLaser::StrikeThink );
	pev->flags |= FL_CUSTOMENTITY;

	PointsInit( GetAbsOrigin(), GetAbsOrigin() );

	if( !m_pSprite && m_iszSpriteName )
		m_pSprite = CSprite::SpriteCreate( STRING( m_iszSpriteName ), GetAbsOrigin(), true );
	else
		m_pSprite = NULL;

	if( m_pSprite )
		m_pSprite->SetTransparency( kRenderGlow, pev->rendercolor.x, pev->rendercolor.y, pev->rendercolor.z, pev->renderamt, pev->renderfx );

	if( HasTargetname() && !( pev->spawnflags & SF_BEAM_STARTON ) )
		TurnOff();
	else
		TurnOn();
}

void CLaser::Precache( void )
{
	pev->modelindex = PRECACHE_MODEL( ( char * ) STRING( pev->model ) );
	if( m_iszSpriteName )
		PRECACHE_MODEL( ( char * ) STRING( m_iszSpriteName ) );
}

void CLaser::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "LaserTarget" ) )
	{
		pev->message = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "width" ) )
	{
		SetWidth( ( int ) atof( pkvd->szValue ) );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "NoiseAmplitude" ) )
	{
		SetNoise( atoi( pkvd->szValue ) );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "TextureScroll" ) )
	{
		SetScrollRate( atoi( pkvd->szValue ) );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "texture" ) )
	{
		pev->model = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "EndSprite" ) )
	{
		m_iszSpriteName = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "framestart" ) )
	{
		pev->frame = atoi( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "damage" ) )
	{
		pev->dmg = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBeam::KeyValue( pkvd );
}

void CLaser::TurnOn( void )
{
	pev->effects &= ~EF_NODRAW;
	if( m_pSprite )
		m_pSprite->TurnOn();
	pev->dmgtime = gpGlobals->time;
	pev->nextthink = gpGlobals->time;
}

void CLaser::TurnOff( void )
{
	pev->effects |= EF_NODRAW;
	pev->nextthink = 0;
	if( m_pSprite )
		m_pSprite->TurnOff();
}

bool CLaser::IsOn() const
{
	if( pev->effects & EF_NODRAW )
		return false;
	return true;
}

void CLaser::FireAtPoint( TraceResult &tr )
{
	SetEndPos( tr.vecEndPos );
	if( m_pSprite )
		m_pSprite->SetAbsOrigin( tr.vecEndPos );

	BeamDamage( &tr );
	DoSparks( GetStartPos(), tr.vecEndPos );
}

void CLaser::StrikeThink( void )
{
	CBaseEntity *pEnd = UTIL_RandomTargetname( STRING( pev->message ) );

	if( pEnd )
		m_firePosition = pEnd->GetAbsOrigin();

	TraceResult tr;

	UTIL_TraceLine( GetAbsOrigin(), m_firePosition, dont_ignore_monsters, NULL, &tr );
	FireAtPoint( tr );
	pev->nextthink = gpGlobals->time + 0.1;
}

void CLaser::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	const bool active = IsOn();

	if( !ShouldToggle( useType, active ) )
		return;
	if( active )
	{
		TurnOff();
	}
	else
	{
		TurnOn();
	}
}