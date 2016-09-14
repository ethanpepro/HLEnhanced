#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CPathCorner.h"

#include "CGunTarget.h"

BEGIN_DATADESC( CGunTarget )
	DEFINE_FIELD( m_on, FIELD_BOOLEAN ),
	DEFINE_THINKFUNC( Next ),
	DEFINE_THINKFUNC( Start ),
	DEFINE_THINKFUNC( Wait ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( func_guntarget, CGunTarget );

void CGunTarget::Spawn( void )
{
	pev->solid = SOLID_BSP;
	pev->movetype = MOVETYPE_PUSH;

	SetAbsOrigin( GetAbsOrigin() );
	SetModel( STRING( pev->model ) );

	if( pev->speed == 0 )
		pev->speed = 100;

	// Don't take damage until "on"
	pev->takedamage = DAMAGE_NO;
	pev->flags |= FL_MONSTER;

	m_on = false;
	pev->max_health = pev->health;

	if( pev->spawnflags & FGUNTARGET_START_ON )
	{
		SetThink( &CGunTarget::Start );
		pev->nextthink = pev->ltime + 0.3;
	}
}

void CGunTarget::Activate( void )
{
	CBaseEntity	*pTarg;

	// now find our next target
	pTarg = GetNextTarget();
	if( pTarg )
	{
		m_hTargetEnt = pTarg;
		SetAbsOrigin( pTarg->GetAbsOrigin() - ( pev->mins + pev->maxs ) * 0.5 );
	}
}

void CGunTarget::Next( void )
{
	SetThink( NULL );

	m_hTargetEnt = GetNextTarget();
	CBaseEntity *pTarget = m_hTargetEnt;

	if( !pTarget )
	{
		Stop();
		return;
	}
	SetMoveDone( &CGunTarget::Wait );
	LinearMove( pTarget->GetAbsOrigin() - ( pev->mins + pev->maxs ) * 0.5, pev->speed );
}

void CGunTarget::Start( void )
{
	Use( this, this, USE_ON, 0 );
}

void CGunTarget::Wait( void )
{
	CBaseEntity *pTarget = m_hTargetEnt;

	if( !pTarget )
	{
		Stop();
		return;
	}

	// Fire the pass target if there is one
	if( pTarget->pev->message )
	{
		FireTargets( STRING( pTarget->pev->message ), this, this, USE_TOGGLE, 0 );
		if( FBitSet( pTarget->pev->spawnflags, SF_CORNER_FIREONCE ) )
			pTarget->pev->message = 0;
	}

	m_flWait = pTarget->GetDelay();

	pev->target = pTarget->pev->target;
	SetThink( &CGunTarget::Next );
	if( m_flWait != 0 )
	{// -1 wait will wait forever!		
		pev->nextthink = pev->ltime + m_flWait;
	}
	else
	{
		Next();// do it RIGHT now!
	}
}

void CGunTarget::Stop( void )
{
	pev->velocity = g_vecZero;
	pev->nextthink = 0;
	pev->takedamage = DAMAGE_NO;
}

void CGunTarget::OnTakeDamage( const CTakeDamageInfo& info )
{
	if( pev->health > 0 )
	{
		pev->health -= info.GetDamage();
		if( pev->health <= 0 )
		{
			pev->health = 0;
			Stop();
			if( pev->message )
				FireTargets( STRING( pev->message ), this, this, USE_TOGGLE, 0 );
		}
	}
}

void CGunTarget::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( !ShouldToggle( useType, m_on ) )
		return;

	if( m_on )
	{
		Stop();
	}
	else
	{
		//TODO: m_on is never turned on here. - Solokiller
		pev->takedamage = DAMAGE_AIM;
		m_hTargetEnt = GetNextTarget();
		if( m_hTargetEnt == NULL )
			return;
		pev->health = pev->max_health;
		Next();
	}
}