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
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CBasePlayer.h"

#include "CRopeSample.h"
#include "CRope.h"

#include "CRopeSegment.h"

BEGIN_DATADESC( CRopeSegment )
	DEFINE_FIELD( m_Sample, FIELD_CLASSPTR ),
	DEFINE_FIELD( mModelName, FIELD_STRING ),
	DEFINE_FIELD( mDefaultMass, FIELD_FLOAT ),
	DEFINE_FIELD( mCauseDamage, FIELD_BOOLEAN ),
	DEFINE_FIELD( mCanBeGrabbed, FIELD_BOOLEAN ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( rope_segment, CRopeSegment );

CRopeSegment::CRopeSegment()
{
	mModelName = MAKE_STRING( "models/rope16.mdl" );
}

void CRopeSegment::Precache()
{
	BaseClass::Precache();

	PRECACHE_MODEL( STRING( mModelName ) );
	PRECACHE_SOUND( "items/grab_rope.wav" );
}

void CRopeSegment::Spawn()
{
	pev->classname = MAKE_STRING( "rope_segment" );

	Precache();

	SetModel( STRING( mModelName ) );

	SetMoveType( MOVETYPE_NOCLIP );
	SetSolidType( SOLID_TRIGGER );
	AddFlags( FL_ALWAYSTHINK );
	SetEffects( EF_NODRAW );
	SetAbsOrigin( GetAbsOrigin() );

	SetSize( Vector( -30, -30, -30 ), Vector( 30, 30, 30 ) );

	SetNextThink( gpGlobals->time + 0.5 );
}

void CRopeSegment::Think()
{
	//Do nothing.
}

void CRopeSegment::Touch( CBaseEntity* pOther )
{
	if( pOther->IsPlayer() )
	{
		auto pPlayer = static_cast<CBasePlayer*>( pOther );

		//Electrified wires deal damage. - Solokiller
		if( mCauseDamage )
		{
			pOther->TakeDamage( CTakeDamageInfo( this, this, 1, DMG_SHOCK ) );
		}

		if( m_Sample->GetMasterRope()->IsAcceptingAttachment() && !pPlayer->IsOnRope() )
		{
			if( mCanBeGrabbed )
			{
				auto& data = m_Sample->GetData();

				pOther->SetAbsOrigin( data.mPosition );

				pPlayer->SetOnRopeState( true );
				pPlayer->SetRope( m_Sample->GetMasterRope() );
				m_Sample->GetMasterRope()->AttachObjectToSegment( this );

				const Vector& vecVelocity = pOther->GetAbsVelocity();

				if( vecVelocity.Length() > 0.5 )
				{
					//Apply some external force to move the rope. - Solokiller
					data.mApplyExternalForce = true;

					data.mExternalForce = data.mExternalForce + vecVelocity * 750;
				}

				if( m_Sample->GetMasterRope()->IsSoundAllowed() )
				{
					EMIT_SOUND( edict(), CHAN_BODY, "items/grab_rope.wav", 1.0, ATTN_NORM );
				}
			}
			else
			{
				//This segment cannot be grabbed, so grab the highest one if possible. - Solokiller
				auto pRope = m_Sample->GetMasterRope();

				CRopeSegment* pSegment;

				if( pRope->GetNumSegments() <= 4 )
				{
					//Fewer than 5 segments exist, so allow grabbing the last one. - Solokiller
					pSegment = pRope->GetSegments()[ pRope->GetNumSegments() - 1 ];
					pSegment->SetCanBeGrabbed( true );
				}
				else
				{
					pSegment = pRope->GetSegments()[ 4 ];
				}

				pSegment->Touch( pOther );
			}
		}
	}
}

CRopeSegment* CRopeSegment::CreateSegment( CRopeSample* pSample, string_t iszModelName )
{
	CRopeSegment* pSegment = GetClassPtr<CRopeSegment>( nullptr );

	pSegment->mModelName = iszModelName;

	pSegment->Spawn();

	pSegment->m_Sample = pSample;

	pSegment->mCauseDamage = false;
	pSegment->mCanBeGrabbed = true;
	pSegment->mDefaultMass = pSample->GetData().mMassReciprocal;

	return pSegment;
}

void CRopeSegment::ApplyExternalForce( const Vector& vecForce )
{
	m_Sample->GetData().mApplyExternalForce = true;

	m_Sample->GetData().mExternalForce = m_Sample->GetData().mExternalForce + vecForce;
}

void CRopeSegment::SetMassToDefault()
{
	m_Sample->GetData().mMassReciprocal = mDefaultMass;
}

void CRopeSegment::SetDefaultMass( const float flDefaultMass )
{
	mDefaultMass = flDefaultMass;
}

void CRopeSegment::SetMass( const float flMass )
{
	m_Sample->GetData().mMassReciprocal = flMass;
}

void CRopeSegment::SetCauseDamageOnTouch( const bool bCauseDamage )
{
	mCauseDamage = bCauseDamage;
}

void CRopeSegment::SetCanBeGrabbed( const bool bCanBeGrabbed )
{
	mCanBeGrabbed = bCanBeGrabbed;
}