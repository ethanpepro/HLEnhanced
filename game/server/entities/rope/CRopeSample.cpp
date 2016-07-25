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

#include "CRopeSample.h"

BEGIN_DATADESC( CRopeSample )
	DEFINE_FIELD( data.mPosition, FIELD_VECTOR ),
	DEFINE_FIELD( data.mVelocity, FIELD_VECTOR ),
	DEFINE_FIELD( data.mForce, FIELD_VECTOR ),
	DEFINE_FIELD( data.mExternalForce, FIELD_VECTOR ),
	DEFINE_FIELD( data.mApplyExternalForce, FIELD_BOOLEAN ),
	DEFINE_FIELD( data.mMassReciprocal, FIELD_FLOAT ),
	DEFINE_FIELD( mMasterRope, FIELD_CLASSPTR ),
END_DATADESC()

LINK_ENTITY_TO_CLASS( rope_sample, CRopeSample );

void CRopeSample::Spawn()
{
	pev->classname = MAKE_STRING( "rope_sample" );

	AddEffectsFlags( EF_NODRAW );
}

CRopeSample* CRopeSample::CreateSample()
{
	CRopeSample* pSample = GetClassPtr<CRopeSample>( nullptr );

	pSample->Spawn();

	return pSample;
}