#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CBasePlayer.h"
#include "entities/plats/CPathCorner.h"

#include "CTriggerCamera.h"

// Global Savedata for changelevel friction modifier
BEGIN_DATADESC( CTriggerCamera )
	DEFINE_FIELD( m_hPlayer, FIELD_EHANDLE ),
	DEFINE_FIELD( m_hTarget, FIELD_EHANDLE ),
	DEFINE_FIELD( m_pentPath, FIELD_CLASSPTR ),
	DEFINE_FIELD( m_sPath, FIELD_STRING ),
	DEFINE_FIELD( m_flWait, FIELD_FLOAT ),
	DEFINE_FIELD( m_flReturnTime, FIELD_TIME ),
	DEFINE_FIELD( m_flStopTime, FIELD_TIME ),
	DEFINE_FIELD( m_moveDistance, FIELD_FLOAT ),
	DEFINE_FIELD( m_targetSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( m_initialSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( m_acceleration, FIELD_FLOAT ),
	DEFINE_FIELD( m_deceleration, FIELD_FLOAT ),
	DEFINE_FIELD( m_state, FIELD_BOOLEAN ),

	DEFINE_THINKFUNC( FollowTarget ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( trigger_camera, CTriggerCamera );

void CTriggerCamera::Spawn( void )
{
	pev->movetype = MOVETYPE_NOCLIP;
	pev->solid = SOLID_NOT;							// Remove model & collisions
	pev->renderamt = 0;								// The engine won't draw this model if this is set to 0 and blending is on
	pev->rendermode = kRenderTransTexture;

	m_initialSpeed = pev->speed;
	if( m_acceleration == 0 )
		m_acceleration = 500;
	if( m_deceleration == 0 )
		m_deceleration = 500;
}

void CTriggerCamera::KeyValue( KeyValueData *pkvd )
{
	if( FStrEq( pkvd->szKeyName, "wait" ) )
	{
		m_flWait = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "moveto" ) )
	{
		m_sPath = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "acceleration" ) )
	{
		m_acceleration = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else if( FStrEq( pkvd->szKeyName, "deceleration" ) )
	{
		m_deceleration = atof( pkvd->szValue );
		pkvd->fHandled = true;
	}
	else
		CBaseDelay::KeyValue( pkvd );
}

void CTriggerCamera::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( !ShouldToggle( useType, m_state ) )
		return;

	// Toggle state
	m_state = !m_state;
	if( !m_state )
	{
		m_flReturnTime = gpGlobals->time;
		return;
	}
	if( !pActivator || !pActivator->IsPlayer() )
	{
		pActivator = CBaseEntity::Instance( g_engfuncs.pfnPEntityOfEntIndex( 1 ) );
	}

	m_hPlayer = pActivator;

	CBasePlayer* pPlayer = static_cast<CBasePlayer*>( pActivator );

	m_flReturnTime = gpGlobals->time + m_flWait;
	pev->speed = m_initialSpeed;
	m_targetSpeed = m_initialSpeed;

	if( FBitSet( pev->spawnflags, SF_CAMERA_PLAYER_TARGET ) )
	{
		m_hTarget = m_hPlayer;
	}
	else
	{
		m_hTarget = GetNextTarget();
	}

	// Nothing to look at!
	if( m_hTarget == NULL )
	{
		return;
	}


	if( FBitSet( pev->spawnflags, SF_CAMERA_PLAYER_TAKECONTROL ) )
	{
		pPlayer->EnableControl( false );
	}

	if( m_sPath )
	{
		//TODO: trigger_camera uses path_corner, so check the classname? - Solokiller
		m_pentPath = UTIL_FindEntityByTargetname( nullptr, STRING( m_sPath ) );
	}
	else
	{
		m_pentPath = NULL;
	}

	m_flStopTime = gpGlobals->time;
	if( m_pentPath )
	{
		if( m_pentPath->pev->speed != 0 )
			m_targetSpeed = m_pentPath->pev->speed;

		m_flStopTime += m_pentPath->GetDelay();
	}

	// copy over player information
	if( FBitSet( pev->spawnflags, SF_CAMERA_PLAYER_POSITION ) )
	{
		SetAbsOrigin( pPlayer->GetAbsOrigin() + pPlayer->pev->view_ofs );
		pev->angles.x = -pPlayer->pev->angles.x;
		pev->angles.y = pPlayer->pev->angles.y;
		pev->angles.z = 0;
		pev->velocity = pPlayer->pev->velocity;
	}
	else
	{
		pev->velocity = Vector( 0, 0, 0 );
	}

	SET_VIEW( pPlayer->edict(), edict() );

	pPlayer->m_hCamera = this;

	SetModel( STRING( pPlayer->pev->model ) );

	// follow the player down
	SetThink( &CTriggerCamera::FollowTarget );
	pev->nextthink = gpGlobals->time;

	m_moveDistance = 0;
	Move();
}

void CTriggerCamera::FollowTarget()
{
	if( m_hPlayer == NULL )
		return;

	CBasePlayer* pPlayer = EHANDLE_cast<CBasePlayer*>( m_hPlayer );

	if( m_hTarget == NULL || m_flReturnTime < gpGlobals->time )
	{
		if( pPlayer->IsAlive() )
		{
			SET_VIEW( pPlayer->edict(), pPlayer->edict() );
			pPlayer->EnableControl( true );
			pPlayer->m_hCamera = nullptr;
		}
		SUB_UseTargets( this, USE_TOGGLE, 0 );
		pev->avelocity = Vector( 0, 0, 0 );
		m_state = false;
		return;
	}

	Vector vecGoal = UTIL_VecToAngles( m_hTarget->GetAbsOrigin() - GetAbsOrigin() );
	vecGoal.x = -vecGoal.x;

	if( pev->angles.y > 360 )
		pev->angles.y -= 360;

	if( pev->angles.y < 0 )
		pev->angles.y += 360;

	float dx = vecGoal.x - pev->angles.x;
	float dy = vecGoal.y - pev->angles.y;

	if( dx < -180 )
		dx += 360;
	if( dx > 180 )
		dx = dx - 360;

	if( dy < -180 )
		dy += 360;
	if( dy > 180 )
		dy = dy - 360;

	pev->avelocity.x = dx * 40 * gpGlobals->frametime;
	pev->avelocity.y = dy * 40 * gpGlobals->frametime;


	if( !( FBitSet( pev->spawnflags, SF_CAMERA_PLAYER_TAKECONTROL ) ) )
	{
		pev->velocity = pev->velocity * 0.8;
		if( pev->velocity.Length() < 10.0 )
			pev->velocity = g_vecZero;
	}

	pev->nextthink = gpGlobals->time;

	Move();
}

void CTriggerCamera::Move()
{
	// Not moving on a path, return
	if( !m_pentPath )
		return;

	// Subtract movement from the previous frame
	m_moveDistance -= pev->speed * gpGlobals->frametime;

	// Have we moved enough to reach the target?
	if( m_moveDistance <= 0 )
	{
		// Fire the passtarget if there is one
		if( m_pentPath->pev->message )
		{
			FireTargets( STRING( m_pentPath->pev->message ), this, this, USE_TOGGLE, 0 );
			if( FBitSet( m_pentPath->pev->spawnflags, SF_CORNER_FIREONCE ) )
				m_pentPath->pev->message = 0;
		}
		// Time to go to the next target
		m_pentPath = m_pentPath->GetNextTarget();

		// Set up next corner
		if( !m_pentPath )
		{
			pev->velocity = g_vecZero;
		}
		else
		{
			if( m_pentPath->pev->speed != 0 )
				m_targetSpeed = m_pentPath->pev->speed;

			Vector delta = m_pentPath->GetAbsOrigin() - GetAbsOrigin();
			m_moveDistance = delta.Length();
			pev->movedir = delta.Normalize();
			m_flStopTime = gpGlobals->time + m_pentPath->GetDelay();
		}
	}

	if( m_flStopTime > gpGlobals->time )
		pev->speed = UTIL_Approach( 0, pev->speed, m_deceleration * gpGlobals->frametime );
	else
		pev->speed = UTIL_Approach( m_targetSpeed, pev->speed, m_acceleration * gpGlobals->frametime );

	float fraction = 2 * gpGlobals->frametime;
	pev->velocity = ( ( pev->movedir * pev->speed ) * fraction ) + ( pev->velocity * ( 1 - fraction ) );
}