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
#include <utility>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CRopeSample.h"
#include "CRopeSegment.h"

#include "CRope.h"

static const char* const g_pszCreakSounds[] = 
{
	"items/rope1.wav",
	"items/rope2.wav",
	"items/rope3.wav"
};

BEGIN_DATADESC( CRope )
	DEFINE_FIELD( m_iSegments, FIELD_INTEGER ),
	DEFINE_FIELD( m_bToggle, FIELD_BOOLEAN ),
	DEFINE_FIELD( m_InitialDeltaTime, FIELD_BOOLEAN ),
	DEFINE_FIELD( mLastTime, FIELD_TIME ),
	DEFINE_FIELD( m_LastEndPos, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_Gravity, FIELD_VECTOR ),
	DEFINE_FIELD( m_HookConstant, FIELD_FLOAT ),
	DEFINE_FIELD( m_SpringDampning, FIELD_FLOAT ),
	DEFINE_FIELD( m_NumSamples, FIELD_INTEGER ),
	DEFINE_FIELD( m_SpringCnt, FIELD_INTEGER ),
	DEFINE_FIELD( mObjectAttached, FIELD_BOOLEAN ),
	DEFINE_FIELD( mAttachedObjectsSegment, FIELD_INTEGER ),
	DEFINE_FIELD( detachTime, FIELD_TIME ),
	DEFINE_ARRAY( seg, FIELD_CLASSPTR, CRope::MAX_SEGMENTS ),
	DEFINE_ARRAY( altseg, FIELD_CLASSPTR, CRope::MAX_SEGMENTS ),
	DEFINE_ARRAY( m_CurrentSys, FIELD_CLASSPTR, CRope::MAX_SAMPLES ),
	DEFINE_ARRAY( m_TargetSys, FIELD_CLASSPTR, CRope::MAX_SAMPLES ),
	DEFINE_FIELD( mDisallowPlayerAttachment, FIELD_INTEGER ),
	DEFINE_FIELD( mBodyModel, FIELD_STRING ),
	DEFINE_FIELD( mEndingModel, FIELD_STRING ),
	DEFINE_FIELD( mAttachedObjectsOffset, FIELD_FLOAT ),
	DEFINE_FIELD( m_bMakeSound, FIELD_BOOLEAN ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( env_rope, CRope );

CRope::CRope()
{
	mBodyModel = MAKE_STRING( "models/rope16.mdl" );
	mEndingModel = MAKE_STRING( "models/rope16.mdl" );
}

CRope::~CRope()
{
	for( size_t uiIndex = 0; uiIndex < MAX_TEMP_SAMPLES; ++uiIndex )
	{
		delete[] m_TempSys[ uiIndex ];
		m_TempSys[ uiIndex ] = nullptr;
	}
}

void CRope::KeyValue( KeyValueData* pkvd )
{
	if( FStrEq( pkvd->szKeyName, "segments" ) )
	{
		pkvd->fHandled = true;

		m_iSegments = strtol( pkvd->szValue, nullptr, 10 );

		if( m_iSegments >= MAX_SEGMENTS )
			m_iSegments = MAX_SEGMENTS - 1;
	}
	else if( FStrEq( pkvd->szKeyName, "bodymodel" ) )
	{
		pkvd->fHandled = true;

		mBodyModel = ALLOC_STRING( pkvd->szValue );
	}
	else if( FStrEq( pkvd->szKeyName, "endingmodel" ) )
	{
		pkvd->fHandled = true;

		mEndingModel = ALLOC_STRING( pkvd->szValue );
	}
	else if( FStrEq( pkvd->szKeyName, "disable" ) )
	{
		pkvd->fHandled = true;

		mDisallowPlayerAttachment = strtol( pkvd->szValue, nullptr, 10 );
	}
	else
		BaseClass::KeyValue( pkvd );
}

void CRope::Precache()
{
	BaseClass::Precache();

	UTIL_PrecacheOther( "rope_segment" );
	UTIL_PrecacheOther( "rope_sample" );

	PRECACHE_SOUND_ARRAY( g_pszCreakSounds );
}

void CRope::Spawn()
{
	pev->classname = MAKE_STRING( "env_rope" );

	m_bMakeSound = true;

	Precache();

	mSpringsInitialized = false;

	m_Gravity.x = m_Gravity.y = 0;
	m_Gravity.z = -50;

	mObjectAttached = false;

	AddFlags( FL_ALWAYSTHINK );
	m_NumSamples = m_iSegments + 1;

	for( size_t uiSample = 0; uiSample < m_NumSamples; ++uiSample )
	{
		m_CurrentSys[ uiSample ] = CRopeSample::CreateSample();

		m_CurrentSys[ uiSample ]->SetMasterRope( this );
	}

	memset( m_CurrentSys + m_NumSamples, 0, sizeof( CRopeSample* ) * ( MAX_SAMPLES - m_NumSamples ) );

	{
		CRopeSegment* pSegment = seg[ 0 ] = CRopeSegment::CreateSegment( m_CurrentSys[ 0 ], GetBodyModel() );

		pSegment->SetAbsOrigin( GetAbsOrigin() );

		pSegment = altseg[ 0 ] = CRopeSegment::CreateSegment( m_CurrentSys[ 0 ], GetBodyModel() );

		pSegment->SetAbsOrigin( GetAbsOrigin() );
	}

	Vector origin;
	Vector angles;

	const Vector vecGravity = m_Gravity.Normalize();

	if( m_iSegments > 2 )
	{
		CRopeSample** ppCurrentSys = m_CurrentSys;

		for( size_t uiSeg = 1; uiSeg < m_iSegments - 1; ++uiSeg )
		{
			CRopeSample* pSegSample = m_CurrentSys[ uiSeg ];
			seg[ uiSeg ] = CRopeSegment::CreateSegment( pSegSample, GetBodyModel() );

			altseg[ uiSeg ] = CRopeSegment::CreateSegment( pSegSample, GetBodyModel() );

			CRopeSegment* pCurrent = seg[ uiSeg - 1 ];

			pCurrent->GetAttachment( 0, origin, angles );

			Vector vecPos = origin - pCurrent->GetAbsOrigin();

			const float flLength = vecPos.Length();

			origin = flLength * vecGravity + pCurrent->GetAbsOrigin();

			seg[ uiSeg ]->SetAbsOrigin( origin );
			altseg[ uiSeg ]->SetAbsOrigin( origin );
		}
	}

	CRopeSample* pSegSample = m_CurrentSys[ m_iSegments - 1 ];
	seg[ m_iSegments - 1 ] = CRopeSegment::CreateSegment( pSegSample, GetEndingModel() );

	altseg[ m_iSegments - 1 ] = CRopeSegment::CreateSegment( pSegSample, GetEndingModel() );

	CRopeSegment* pCurrent = seg[ m_iSegments - 2 ];

	pCurrent->GetAttachment( 0, origin, angles );

	Vector vecPos = origin - pCurrent->GetAbsOrigin();

	const float flLength = vecPos.Length();

	origin = flLength * vecGravity + pCurrent->GetAbsOrigin();

	seg[ m_iSegments - 1 ]->SetAbsOrigin( origin );
	altseg[ m_iSegments - 1 ]->SetAbsOrigin( origin );

	memset( seg + m_iSegments, 0, sizeof( CRopeSegment* ) * ( MAX_SEGMENTS - m_iSegments ) );
	memset( altseg + m_iSegments, 0, sizeof( CRopeSegment* ) * ( MAX_SEGMENTS - m_iSegments ) );

	memset( m_TempSys, 0, sizeof( m_TempSys ) );

	m_SpringCnt = 0;

	m_InitialDeltaTime = true;
	m_HookConstant = 2500;
	m_SpringDampning = 0.1;

	InitializeRopeSim();

	SetNextThink( gpGlobals->time + 0.01 );
}

void CRope::Think()
{
	if( !mSpringsInitialized )
	{
		InitializeSprings( m_iSegments );
	}

	m_bToggle = !m_bToggle;

	RunSimOnSamples();

	CRopeSegment** ppPrimarySegs;
	CRopeSegment** ppHiddenSegs;

	if( m_bToggle )
	{
		ppPrimarySegs = altseg;
		ppHiddenSegs = seg;
	}
	else
	{
		ppPrimarySegs = seg;
		ppHiddenSegs = altseg;
	}

	SetRopeSegments( m_iSegments, ppPrimarySegs, ppHiddenSegs );

	if( ShouldCreak() )
	{
		Creak();
	}

	SetNextThink( gpGlobals->time + 0.001 );
}

void CRope::Touch( CBaseEntity* pOther )
{
	//Nothing.
}

bool CRope::Restore( CRestore& restore )
{
	if( !BaseClass::Restore( restore ) )
	{
		return false;
	}

	for( size_t uiIndex = 0; uiIndex < MAX_TEMP_SAMPLES; ++uiIndex )
	{
		m_TempSys[ uiIndex ] = new RopeSampleData[ m_NumSamples ];
	}

	mSpringsInitialized = false;
	m_InitialDeltaTime = true;

	return true;
}

void CRope::InitializeRopeSim()
{
	for( size_t uiIndex = 0; uiIndex < MAX_TEMP_SAMPLES; ++uiIndex )
	{
		delete[] m_TempSys[ uiIndex ];
		m_TempSys[ uiIndex ] = nullptr;
	}

	for( size_t uiSample = 0; uiSample < m_NumSamples; ++uiSample )
	{
		m_TargetSys[ uiSample ] = CRopeSample::CreateSample();

		m_TargetSys[ uiSample ]->SetMasterRope( this );
	}

	memset( m_TargetSys + m_NumSamples, 0, sizeof( CRopeSample* ) * ( MAX_SAMPLES - m_NumSamples ) );

	for( size_t uiIndex = 0; uiIndex < MAX_TEMP_SAMPLES; ++uiIndex )
	{
		m_TempSys[ uiIndex ] = new RopeSampleData[ m_NumSamples ];

		memset( m_TempSys[ uiIndex ], 0, sizeof( RopeSampleData ) * m_NumSamples );
	}

	for( size_t uiSeg = 0; uiSeg < m_iSegments; ++uiSeg )
	{
		CRopeSegment* pSegment = seg[ uiSeg ];
		CRopeSample* pSample = pSegment->GetSample();

		auto& data = pSample->GetData();

		data.mPosition = pSegment->GetAbsOrigin();

		data.mVelocity				= g_vecZero;
		data.mForce					= g_vecZero;
		data.mMassReciprocal		= 1;
		data.mApplyExternalForce	= false;
		data.mExternalForce			= g_vecZero;

		pSegment->SetDefaultMass( data.mMassReciprocal );
	}

	{
		//Zero out the anchored segment's mass so it stays in place.
		auto pSample = m_CurrentSys[ 0 ];

		pSample->GetData().mMassReciprocal = 0;
	}

	CRopeSegment* pSegment = seg[ m_iSegments - 1 ];

	Vector vecOrigin, vecAngles;

	pSegment->GetAttachment( 0, vecOrigin, vecAngles );

	Vector vecDistance = vecOrigin - pSegment->GetAbsOrigin();

	const float flLength = vecDistance.Length();

	const Vector vecGravity = m_Gravity.Normalize();

	vecOrigin = vecGravity * flLength + pSegment->GetAbsOrigin();

	CRopeSample* pSample = m_CurrentSys[ m_NumSamples - 1 ];

	auto& data = pSample->GetData();

	data.mPosition = vecOrigin;

	m_LastEndPos = vecOrigin;

	data.mVelocity = g_vecZero;

	data.mForce = g_vecZero;

	data.mMassReciprocal = 0.2;

	data.mApplyExternalForce = false;

	size_t uiNumSegs = 4;

	if( m_iSegments <= 4 )
		uiNumSegs = m_iSegments;

	for( size_t uiIndex = 0; uiIndex < uiNumSegs; ++uiIndex )
	{
		seg[ uiIndex ]->SetCanBeGrabbed( false );
		altseg[ uiIndex ]->SetCanBeGrabbed( false );
	}
}

void CRope::InitializeSprings( const size_t uiNumSprings )
{
	m_SpringCnt = uiNumSprings;

	m_Spring = new Spring[ uiNumSprings ];

	if( uiNumSprings > 0 )
	{
		Vector vecOrigin, vecAngles;

		for( size_t uiIndex = 0; uiIndex < m_SpringCnt; ++uiIndex )
		{
			Spring& spring = m_Spring[ uiIndex ];

			spring.p1 = uiIndex;
			spring.p2 = uiIndex + 1;

			if( uiIndex < m_iSegments )
			{
				CRopeSegment* pSegment = seg[ uiIndex ];

				pSegment->GetAttachment( 0, vecOrigin, vecAngles );

				spring.restLength = ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
			}
			else
				spring.restLength = 0;

			spring.hookConstant = m_HookConstant;
			spring.springDampning = m_SpringDampning;
		}
	}

	mSpringsInitialized = true;
}

void CRope::RunSimOnSamples()
{
	float flDeltaTime = 0.025;

	if( m_InitialDeltaTime )
	{
		m_InitialDeltaTime = false;
		mLastTime = gpGlobals->time;
		flDeltaTime = 0;
	}

	size_t uiIndex = 0;

	CRopeSample** ppSampleSource = m_CurrentSys;
	CRopeSample** ppSampleTarget = m_TargetSys;

	while( true )
	{
		++uiIndex;

		ComputeForces( ppSampleSource );
		RK4Integrate( flDeltaTime, ppSampleSource, ppSampleTarget );

		mLastTime += 0.007;

		if( gpGlobals->time <= mLastTime )
		{
			if( ( uiIndex % 2 ) != 0 )
				break;
		}

		std::swap( ppSampleSource, ppSampleTarget );
	}

	mLastTime = gpGlobals->time;
}

void CRope::ComputeForces( RopeSampleData* pSystem )
{
	for( size_t uiIndex = 0; uiIndex < m_NumSamples; ++uiIndex )
	{
		ComputeSampleForce( pSystem[ uiIndex ] );
	}

	Spring* pSpring = m_Spring;

	for( size_t uiIndex = 0; uiIndex < m_SpringCnt; ++uiIndex, ++pSpring )
	{
		ComputeSpringForce( pSystem[ pSpring->p1 ], pSystem[ pSpring->p2 ], *pSpring );
	}
}

void CRope::ComputeForces( CRopeSample** ppSystem )
{
	for( size_t uiIndex = 0; uiIndex < m_NumSamples; ++uiIndex )
	{
		ComputeSampleForce( ppSystem[ uiIndex ]->GetData() );
	}

	Spring* pSpring = m_Spring;

	for( size_t uiIndex = 0; uiIndex < m_SpringCnt; ++uiIndex, ++pSpring )
	{
		ComputeSpringForce( ppSystem[ pSpring->p1 ]->GetData(), ppSystem[ pSpring->p2 ]->GetData(), *pSpring );
	}
}

void CRope::ComputeSampleForce( RopeSampleData& data )
{
	data.mForce = g_vecZero;

	if( data.mMassReciprocal != 0.0 )
	{
		data.mForce = data.mForce + ( m_Gravity / data.mMassReciprocal );
	}

	if( data.mApplyExternalForce )
	{
		data.mForce = data.mForce + data.mExternalForce;

		data.mExternalForce = g_vecZero;
		data.mApplyExternalForce = false;
	}

	if( DotProduct( m_Gravity, data.mVelocity ) >= 0 )
	{
		data.mForce = data.mForce + data.mVelocity * -0.04;
	}
	else
	{
		data.mForce = data.mForce - data.mVelocity;
	}
}

void CRope::ComputeSpringForce( RopeSampleData& first, RopeSampleData& second, const Spring& spring )
{
	Vector vecDist = first.mPosition - second.mPosition;

	const double flDistance = vecDist.Length();

	const double flForce = ( flDistance - spring.restLength ) * spring.hookConstant;

	const double flNewRelativeDist = DotProduct( first.mVelocity - second.mVelocity, vecDist ) * spring.springDampning;

	vecDist.NormalizeInPlace();

	const double flSpringFactor = -( flNewRelativeDist / flDistance + flForce );

	const Vector vecForce = flSpringFactor * vecDist;

	first.mForce = first.mForce + vecForce;

	second.mForce = second.mForce - vecForce;
}

void CRope::RK4Integrate( const float flDeltaTime, CRopeSample** ppSampleSource, CRopeSample** ppSampleTarget )
{
	const float flDeltas[ MAX_TEMP_SAMPLES - 1 ] = 
	{
		flDeltaTime * 0.5f,
		flDeltaTime * 0.5f,
		flDeltaTime * 0.5f,
		flDeltaTime
	};

	{
		RopeSampleData* pTemp1 = m_TempSys[ 0 ];
		RopeSampleData* pTemp2 = m_TempSys[ 1 ];

		for( size_t uiIndex = 0; uiIndex < m_NumSamples; ++uiIndex, ++pTemp1, ++pTemp2 )
		{
			const auto& data = ppSampleSource[ uiIndex ]->GetData();

			pTemp2->mForce = data.mMassReciprocal * data.mForce * flDeltas[ 0 ];

			pTemp2->mVelocity = data.mVelocity * flDeltas[ 0 ];

			pTemp1->mMassReciprocal = data.mMassReciprocal;
			pTemp1->mVelocity = data.mVelocity + pTemp2->mForce;
			pTemp1->mPosition = data.mPosition + pTemp2->mVelocity;
		}

		ComputeForces( m_TempSys[ 0 ] );
	}

	for( size_t uiStep = 2; uiStep < MAX_TEMP_SAMPLES - 1; ++uiStep )
	{
		RopeSampleData* pTemp1 = m_TempSys[ 0 ];
		RopeSampleData* pTemp2 = m_TempSys[ uiStep ];

		for( size_t uiIndex = 0; uiIndex < m_NumSamples; ++uiIndex, ++pTemp1, ++pTemp2 )
		{
			const auto& data = ppSampleSource[ uiIndex ]->GetData();

			pTemp2->mForce = data.mMassReciprocal * pTemp1->mForce * flDeltas[ uiStep - 1 ];

			pTemp2->mVelocity = pTemp1->mVelocity * flDeltas[ uiStep - 1 ];

			pTemp1->mMassReciprocal = data.mMassReciprocal;
			pTemp1->mVelocity = data.mVelocity + pTemp2->mForce;
			pTemp1->mPosition = data.mPosition + pTemp2->mVelocity;
		}

		ComputeForces( m_TempSys[ 0 ] );
	}

	{
		RopeSampleData* pTemp1 = m_TempSys[ 0 ];
		RopeSampleData* pTemp2 = m_TempSys[ 4 ];

		for( size_t uiIndex = 0; uiIndex < m_NumSamples; ++uiIndex, ++pTemp1, ++pTemp2 )
		{
			const auto& data = ppSampleSource[ uiIndex ]->GetData();

			pTemp2->mForce = data.mMassReciprocal * pTemp1->mForce * flDeltas[ 3 ];

			pTemp2->mVelocity = pTemp1->mVelocity * flDeltas[ 3 ];
		}
	}

	RopeSampleData* pTemp1 = m_TempSys[ 1 ];
	RopeSampleData* pTemp2 = m_TempSys[ 2 ];
	RopeSampleData* pTemp3 = m_TempSys[ 3 ];
	RopeSampleData* pTemp4 = m_TempSys[ 4 ];

	for( size_t uiIndex = 0; uiIndex < m_NumSamples; ++uiIndex, ++pTemp1, ++pTemp2, ++pTemp3, ++pTemp4 )
	{
		auto pSource = ppSampleSource[ uiIndex ];
		auto pTarget = ppSampleTarget[ uiIndex ];

		const Vector vecPosChange = 1.0f / 6.0f * ( pTemp1->mVelocity + ( pTemp2->mVelocity + pTemp3->mVelocity ) * 2 + pTemp4->mVelocity );

		const Vector vecVelChange = 1.0f / 6.0f * ( pTemp1->mForce + ( pTemp2->mForce + pTemp3->mForce ) * 2 + pTemp4->mForce );

		pTarget->GetData().mPosition = pSource->GetData().mPosition + ( vecPosChange );//* flDeltaTime );

		pTarget->GetData().mVelocity = pSource->GetData().mVelocity + ( vecVelChange );//* flDeltaTime );
	}
}

//TODO move to common header - Solokiller
static const Vector DOWN( 0, 0, -1 );

static const Vector RIGHT( 0, 1, 0 );

void GetAlignmentAngles( const Vector& vecTop, const Vector& vecBottom, Vector& vecOut )
{
	Vector vecDist = vecBottom - vecTop;

	Vector vecResult = vecDist.Normalize();

	const float flRoll = acos( DotProduct( vecResult, RIGHT ) ) * ( 180.0 / M_PI );

	vecOut.z = -flRoll;

	vecDist.y = 0;

	vecResult = vecDist.Normalize();

	const float flPitch = acos( DotProduct( vecResult, DOWN ) ) * ( 180.0 / M_PI );

	vecOut.x = ( vecResult.x >= 0.0 ) ? flPitch : -flPitch;
	vecOut.y = 0;
}

void TruncateEpsilon( Vector& vec )
{
	vec = ( vec * 10.0 + 0.5 ) / 10.0;
}

void CRope::TraceModels( CRopeSegment** ppPrimarySegs, CRopeSegment** ppHiddenSegs )
{
	if( m_iSegments > 1 )
	{
		Vector vecAngles;

		GetAlignmentAngles( 
			m_CurrentSys[ 0 ]->GetData().mPosition, 
			m_CurrentSys[ 1 ]->GetData().mPosition, 
			vecAngles );

		( *ppPrimarySegs )->SetAbsAngles( vecAngles );
	}

	TraceResult tr;

	if( mObjectAttached )
	{
		for( size_t uiSeg = 1; uiSeg < m_iSegments; ++uiSeg )
		{
			CRopeSample* pSample = m_CurrentSys[ uiSeg ];

			Vector vecDist = pSample->GetData().mPosition - ppHiddenSegs[ uiSeg ]->GetAbsOrigin();
			
			vecDist.NormalizeInPlace();

			const float flTraceDist = ( uiSeg - mAttachedObjectsSegment + 2 ) < 5 ? 50 : 10;

			const Vector vecTraceDist = vecDist * flTraceDist;

			const Vector vecEnd = pSample->GetData().mPosition + vecTraceDist;

			UTIL_TraceLine( ppHiddenSegs[ uiSeg ]->GetAbsOrigin(), vecEnd, ignore_monsters, edict(), &tr );
			
			if( tr.flFraction == 1.0 && tr.fAllSolid )
			{
				break;
			}

			if( tr.flFraction != 1.0 || tr.fStartSolid || !tr.fInOpen )
			{
				Vector vecOrigin = tr.vecEndPos - vecTraceDist;

				TruncateEpsilon( vecOrigin );

				ppPrimarySegs[ uiSeg ]->SetAbsOrigin( vecOrigin );
				
				Vector vecNormal = tr.vecPlaneNormal.Normalize() * 20000.0;

				auto& data = ppPrimarySegs[ uiSeg ]->GetSample()->GetData();

				data.mApplyExternalForce = true;

				data.mExternalForce = vecNormal;

				data.mVelocity = g_vecZero;
			}
			else
			{
				Vector vecOrigin = pSample->GetData().mPosition;

				TruncateEpsilon( vecOrigin );

				ppPrimarySegs[ uiSeg ]->SetAbsOrigin( vecOrigin );
			}
		}
	}
	else
	{
		for( size_t uiSeg = 1; uiSeg < m_iSegments; ++uiSeg )
		{
			UTIL_TraceLine( 
				ppHiddenSegs[ uiSeg ]->GetAbsOrigin(),
				m_CurrentSys[ uiSeg ]->GetData().mPosition, 
				ignore_monsters, edict(), &tr );

			if( tr.flFraction == 1.0 )
			{
				Vector vecOrigin = m_CurrentSys[ uiSeg ]->GetData().mPosition;

				TruncateEpsilon( vecOrigin );

				ppPrimarySegs[ uiSeg ]->SetAbsOrigin( vecOrigin );
			}
			else
			{
				CBaseEntity* pEnt = GET_PRIVATE( tr.pHit );
				const Vector vecNormal = tr.vecPlaneNormal.Normalize();

				Vector vecOrigin = tr.vecEndPos + vecNormal * 10.0;

				TruncateEpsilon( vecOrigin );

				ppPrimarySegs[ uiSeg ]->SetAbsOrigin( vecOrigin );

				ppPrimarySegs[ uiSeg ]->GetSample()->GetData().mApplyExternalForce = true;

				ppPrimarySegs[ uiSeg ]->GetSample()->GetData().mExternalForce = vecNormal * 40000.0;
			}
		}
	}

	Vector vecAngles;

	for( size_t uiSeg = 1; uiSeg < m_iSegments; ++uiSeg )
	{
		auto pSegment = ppPrimarySegs[ uiSeg - 1 ];
		auto pSegment2 = ppPrimarySegs[ uiSeg ];

		GetAlignmentAngles( pSegment->GetAbsOrigin(), pSegment2->GetAbsOrigin(), vecAngles );

		pSegment->SetAbsAngles( vecAngles );
	}

	if( m_iSegments > 1 )
	{
		auto pSample = m_CurrentSys[ m_NumSamples - 1 ];

		UTIL_TraceLine( m_LastEndPos, pSample->GetData().mPosition, ignore_monsters, edict(), &tr );
	
		if( tr.flFraction == 1.0 )
		{
			m_LastEndPos = pSample->GetData().mPosition;
		}
		else
		{
			m_LastEndPos = tr.vecEndPos;

			pSample->GetData().mApplyExternalForce = true;

			pSample->GetData().mExternalForce = tr.vecPlaneNormal.Normalize() * 40000.0;
		}

		auto pSegment = ppPrimarySegs[ m_NumSamples - 2 ];

		Vector vecAngles;

		GetAlignmentAngles( pSegment->GetAbsOrigin(), m_LastEndPos, vecAngles );

		pSegment->SetAbsAngles( vecAngles );
	}
}

void CRope::SetRopeSegments( const size_t uiNumSegments, 
							 CRopeSegment** ppPrimarySegs, CRopeSegment** ppHiddenSegs )
{
	if( uiNumSegments > 0 )
	{
		TraceModels( ppPrimarySegs, ppHiddenSegs );

		ppPrimarySegs[ 0 ]->SetSolidType( SOLID_TRIGGER );
		ppPrimarySegs[ 0 ]->SetEffects( 0 );

		ppHiddenSegs[ 0 ]->SetSolidType( SOLID_NOT );
		ppHiddenSegs[ 0 ]->SetEffects( EF_NODRAW );

		for( size_t uiIndex = 1; uiIndex < uiNumSegments; ++uiIndex )
		{
			CRopeSegment* pPrim = ppPrimarySegs[ uiIndex ];
			CRopeSegment* pHidden = ppHiddenSegs[ uiIndex ];

			pPrim->SetSolidType( SOLID_TRIGGER );
			pPrim->SetEffects( 0 );

			pHidden->SetSolidType( SOLID_NOT );
			pHidden->SetEffects( EF_NODRAW );

			Vector vecOrigin = pPrim->GetAbsOrigin();

			//vecOrigin.x += 10.0;
			//vecOrigin.y += 10.0;

			pHidden->SetAbsOrigin( vecOrigin );
		}
	}
}

bool CRope::MoveUp( const float flDeltaTime )
{
	if( mAttachedObjectsSegment > 4 )
	{
		float flDistance = flDeltaTime * 128.0;

		Vector vecOrigin, vecAngles;

		while( true )
		{
			float flOldDist = flDistance;

			flDistance = 0;

			if( flOldDist <= 0 )
				break;

			if( mAttachedObjectsSegment <= 3 )
				break;

			if( flOldDist > mAttachedObjectsOffset )
			{
				flDistance = flOldDist - mAttachedObjectsOffset;

				--mAttachedObjectsSegment;

				float flNewOffset = 0;

				if( mAttachedObjectsSegment < m_iSegments )
				{
					auto pSegment = seg[ mAttachedObjectsSegment ];

					pSegment->GetAttachment( 0, vecOrigin, vecAngles );

					flNewOffset = ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
				}

				mAttachedObjectsOffset = flNewOffset;
			}
			else
			{
				mAttachedObjectsOffset -= flOldDist;
			}
		}
	}

	return true;
}

bool CRope::MoveDown( const float flDeltaTime )
{
	if( !mObjectAttached )
		return false;

	float flDistance = flDeltaTime * 128.0;

	Vector vecOrigin, vecAngles;

	CRopeSegment* pSegment;

	bool bOnRope = true;

	bool bDoIteration = true;

	while( bDoIteration )
	{
		bDoIteration = false;

		if( flDistance > 0.0 )
		{
			float flNewDist = flDistance;
			float flSegLength = 0.0;

			while( bOnRope )
			{
				if( mAttachedObjectsSegment < m_iSegments )
				{
					pSegment = seg[ mAttachedObjectsSegment ];

					pSegment->GetAttachment( 0, vecOrigin, vecAngles );

					flSegLength = ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
				}

				const float flOffset = flSegLength - mAttachedObjectsOffset;

				if( flNewDist <= flOffset )
				{
					mAttachedObjectsOffset += flNewDist;
					flDistance = 0;
					bDoIteration = true;
					break;
				}

				if( mAttachedObjectsSegment + 1 == m_iSegments )
					bOnRope = false;
				else
					++mAttachedObjectsSegment;

				flNewDist -= flOffset;
				flSegLength = 0;

				mAttachedObjectsOffset = 0;

				if( flNewDist <= 0 )
					break;
			}
		}
	}

	return bOnRope;
}

Vector CRope::GetAttachedObjectsVelocity() const
{
	if( !mObjectAttached )
		return g_vecZero;

	return seg[ mAttachedObjectsSegment ]->GetSample()->GetData().mVelocity;
}

void CRope::ApplyForceFromPlayer( const Vector& vecForce )
{
	if( !mObjectAttached )
		return;

	float flForce = 20000.0;

	if( m_iSegments < 26 )
		flForce *= ( m_iSegments / 26.0 );

	const Vector vecScaledForce = vecForce * flForce;

	ApplyForceToSegment( vecScaledForce, mAttachedObjectsSegment );
}

void CRope::ApplyForceToSegment( const Vector& vecForce, const size_t uiSegment )
{
	if( uiSegment < m_iSegments )
	{
		seg[ uiSegment ]->ApplyExternalForce( vecForce );
	}
	else if( uiSegment == m_iSegments )
	{
		//Apply force to the last sample.

		auto& data = m_CurrentSys[ uiSegment - 1 ]->GetData();

		data.mExternalForce = data.mExternalForce + vecForce;

		data.mApplyExternalForce = true;
	}
}

void CRope::AttachObjectToSegment( CRopeSegment* pSegment )
{
	mObjectAttached = true;

	detachTime = 0;

	SetAttachedObjectsSegment( pSegment );

	mAttachedObjectsOffset = 0;
}

void CRope::DetachObject()
{
	mObjectAttached = false;
	detachTime = gpGlobals->time;
}

bool CRope::IsAcceptingAttachment() const
{
	if( gpGlobals->time - detachTime > 2.0 && !mObjectAttached )
	{
		return mDisallowPlayerAttachment != 1;
	}

	return false;
}

bool CRope::ShouldCreak() const
{
	if( mObjectAttached && m_bMakeSound )
	{
		CRopeSample* pSample = seg[ mAttachedObjectsSegment ]->GetSample();

		if( pSample->GetData().mVelocity.Length() > 20.0 )
			return UTIL_RandomLong( 1, 5 ) == 1;
	}

	return false;
}

void CRope::Creak()
{
	EMIT_SOUND( edict(), CHAN_BODY,
				g_pszCreakSounds[ UTIL_RandomLong( 0, ARRAYSIZE( g_pszCreakSounds ) - 1 ) ],
				VOL_NORM, ATTN_NORM );
}

float CRope::GetSegmentLength( size_t uiSegmentIndex ) const
{
	if( uiSegmentIndex < m_iSegments )
	{
		Vector vecOrigin, vecAngles;

		auto pSegment = seg[ uiSegmentIndex ];

		pSegment->GetAttachment( 0, vecOrigin, vecAngles );

		return ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
	}

	return 0;
}

float CRope::GetRopeLength() const
{
	float flLength = 0;

	Vector vecOrigin, vecAngles;

	for( size_t uiIndex = 0; uiIndex < m_iSegments; ++uiIndex )
	{
		auto pSegment = seg[ uiIndex ];

		pSegment->GetAttachment( 0, vecOrigin, vecAngles );

		flLength += ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
	}

	return flLength;
}

Vector CRope::GetRopeOrigin() const
{
	return m_CurrentSys[ 0 ]->GetData().mPosition;
}

bool CRope::IsValidSegmentIndex( const size_t uiSegment ) const
{
	return uiSegment < m_iSegments;
}

Vector CRope::GetSegmentOrigin( const size_t uiSegment ) const
{
	if( !IsValidSegmentIndex( uiSegment ) )
		return g_vecZero;

	return m_CurrentSys[ uiSegment ]->GetData().mPosition;
}

Vector CRope::GetSegmentAttachmentPoint( const size_t uiSegment ) const
{
	if( !IsValidSegmentIndex( uiSegment ) )
		return g_vecZero;

	Vector vecOrigin, vecAngles;

	auto pSegment = m_bToggle ? altseg[ uiSegment ] : seg[ uiSegment ];

	pSegment->GetAttachment( 0, vecOrigin, vecAngles );

	return vecOrigin;
}

void CRope::SetAttachedObjectsSegment( CRopeSegment* pSegment )
{
	for( size_t uiIndex = 0; uiIndex < m_iSegments; ++uiIndex )
	{
		if( seg[ uiIndex ] == pSegment || altseg[ uiIndex ] == pSegment )
		{
			mAttachedObjectsSegment = uiIndex;
			break;
		}
	}
}

Vector CRope::GetSegmentDirFromOrigin( const size_t uiSegmentIndex ) const
{
	if( uiSegmentIndex >= m_iSegments )
		return g_vecZero;

	//There is one more sample than there are segments, so this is fine.
	const Vector vecResult = 
		m_CurrentSys[ uiSegmentIndex + 1 ]->GetData().mPosition - 
		m_CurrentSys[ uiSegmentIndex ]->GetData().mPosition;

	return vecResult.Normalize();
}

Vector CRope::GetAttachedObjectsPosition() const
{
	if( !mObjectAttached )
		return g_vecZero;

	Vector vecResult;

	if( mAttachedObjectsSegment < m_iSegments )
		vecResult = m_CurrentSys[ mAttachedObjectsSegment ]->GetData().mPosition;

	vecResult = vecResult + 
		( mAttachedObjectsOffset * GetSegmentDirFromOrigin( mAttachedObjectsSegment ) );

	return vecResult;
}