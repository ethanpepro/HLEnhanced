/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
// mathlib.h
#ifndef PUBLIC_MATH_MATHLIB_H
#define PUBLIC_MATH_MATHLIB_H

typedef float vec_t;

//TODO: replace with Vector4D - Solokiller
typedef vec_t vec4_t[ 4 ];	// x,y,z,w

#include "vector.h"

extern const Vector vec3_origin;

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

// up / down
#define	PITCH	0
// left / right
#define	YAW		1
// fall over
#define	ROLL	2 

extern const int nanmask;

/**
*	@return Whether the given float is Not a Number.
*/
inline bool IS_NAN( const float flValue )
{
	return ( ( ( *( int* ) &flValue ) & nanmask ) == nanmask );
}

float VectorNormalize( Vector& v );		// returns vector length

void VectorMA( const Vector& veca, float scale, const Vector& vecb, Vector& vecc );

void AngleVectors( const Vector& angles, Vector* forward, Vector* right, Vector* up );

/**
*	Convenience function for cases where all 3 vectors are used. No need to pass pointers.
*/
inline void AngleVectors( const Vector& angles, Vector& forward, Vector& right, Vector& up )
{
	AngleVectors( angles, &forward, &right, &up );
}

void AngleVectorsTranspose( const Vector& angles, Vector* forward, Vector* right, Vector* up );

/**
*	Convenience function for cases where all 3 vectors are used. No need to pass pointers.
*/
inline void AngleVectorsTranspose( const Vector& angles, Vector& forward, Vector& right, Vector& up )
{
	AngleVectorsTranspose( angles, &forward, &right, &up );
}

#define AngleIVectors	AngleVectorsTranspose

void VectorMatrix( Vector& forward, Vector& right, Vector& up );

void VectorAngles( const Vector& forward, Vector& angles );

void AngleMatrix( const Vector& angles, float( *matrix )[ 4 ] );

void AngleIMatrix( const Vector& angles, float matrix[ 3 ][ 4 ] );

void VectorTransform( const Vector& in1, float in2[ 3 ][ 4 ], Vector& out );

void NormalizeAngles( float* angles );

float FixAngle( float angle );

void FixupAngles( Vector& v );

float AngleBetweenVectors( const Vector& v1, const Vector& v2 );

float Distance( const Vector& v1, const Vector& v2 );

void InterpolateAngles( Vector& start, Vector&end, Vector& output, float frac );

int Q_log2( int val );

float anglemod( float a );

float UTIL_AngleMod( float a );

float UTIL_AngleDiff( float destAngle, float srcAngle );

Vector UTIL_ClampVectorToBox( const Vector &input, const Vector &clampSize );

float UTIL_Approach( float target, float value, float speed );
float UTIL_ApproachAngle( float target, float value, float speed );
float UTIL_AngleDistance( float next, float cur );

// Use for ease-in, ease-out style interpolation (accel/decel)
float UTIL_SplineFraction( float value, float scale );

// Sorta like FInViewCone, but for nonmonsters. 
extern float UTIL_DotPoints( const Vector &vecSrc, const Vector &vecCheck, const Vector &vecDir );

#define BOX_ON_PLANE_SIDE(emins, emaxs, p)	\
	(((p)->type < 3)?						\
	(										\
		((p)->dist <= (emins)[(p)->type])?	\
			1								\
		:									\
		(									\
			((p)->dist >= (emaxs)[(p)->type])?\
				2							\
			:								\
				3							\
		)									\
	)										\
	:										\
		BoxOnPlaneSide( (emins), (emaxs), (p)))

#endif //PUBLIC_MATH_MATHLIB_H