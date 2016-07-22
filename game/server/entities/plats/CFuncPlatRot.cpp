#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CFuncPlatRot.h"

BEGIN_DATADESC( CFuncPlatRot )
	DEFINE_FIELD( m_end, FIELD_VECTOR ),
	DEFINE_FIELD( m_start, FIELD_VECTOR ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( func_platrot, CFuncPlatRot );

void CFuncPlatRot::Spawn( void )
{
	CFuncPlat::Spawn();
	SetupRotation();
}

void CFuncPlatRot::SetupRotation( void )
{
	if( m_vecFinalAngle.x != 0 )		// This plat rotates too!
	{
		CBaseToggle::AxisDir( this );
		m_start = pev->angles;
		m_end = pev->angles + pev->movedir * m_vecFinalAngle.x;
	}
	else
	{
		m_start = g_vecZero;
		m_end = g_vecZero;
	}
	if( HasTargetname() )	// Start at top
	{
		pev->angles = m_end;
	}
}

//
// Platform is at bottom, now starts moving up
//
void CFuncPlatRot::GoUp( void )
{
	CFuncPlat::GoUp();
	RotMove( m_end, pev->nextthink - pev->ltime );
}

void CFuncPlatRot::GoDown( void )
{
	CFuncPlat::GoDown();
	RotMove( m_start, pev->nextthink - pev->ltime );
}

//
// Platform has hit top.  Pauses, then starts back down again.
//
void CFuncPlatRot::HitTop( void )
{
	CFuncPlat::HitTop();
	pev->avelocity = g_vecZero;
	pev->angles = m_end;
}

//
// Platform has hit bottom.  Stops and waits forever.
//
void CFuncPlatRot::HitBottom( void )
{
	CFuncPlat::HitBottom();
	pev->avelocity = g_vecZero;
	pev->angles = m_start;
}

void CFuncPlatRot::RotMove( Vector &destAngle, float time )
{
	// set destdelta to the vector needed to move
	Vector vecDestDelta = destAngle - pev->angles;

	// Travel time is so short, we're practically there already;  so make it so.
	if( time >= 0.1 )
		pev->avelocity = vecDestDelta / time;
	else
	{
		pev->avelocity = vecDestDelta;
		pev->nextthink = pev->ltime + 1;
	}
}