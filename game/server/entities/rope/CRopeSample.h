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
#ifndef GAME_SERVER_ENTITIES_ROPE_CROPESAMPLE_H
#define GAME_SERVER_ENTITIES_ROPE_CROPESAMPLE_H

class CRope;

/**
*	Data for a single rope joint.
*/
struct RopeSampleData
{
	Vector mPosition;
	Vector mVelocity;
	Vector mForce;
	Vector mExternalForce;

	bool mApplyExternalForce;

	float mMassReciprocal;
};

/**
*	Represents a single joint in a rope. There are numSegments + 1 samples in a rope.
*/
class CRopeSample : public CBaseEntity
{
public:
	DECLARE_CLASS( CRopeSample, CBaseEntity );
	DECLARE_DATADESC();

	void Spawn() override;

	static CRopeSample* CreateSample();

	const RopeSampleData& GetData() const { return data; }

	RopeSampleData& GetData() { return data; }

	CRope* GetMasterRope() { return mMasterRope; }

	void SetMasterRope( CRope* pRope )
	{
		mMasterRope = pRope;
	}

private:
	RopeSampleData data;
	CRope* mMasterRope;
};

#endif //GAME_SERVER_ENTITIES_ROPE_CROPESAMPLE_H