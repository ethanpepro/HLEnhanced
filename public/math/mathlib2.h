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

#include "vector.h"

extern const Vector vec3_origin;

#ifndef M_PI
#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h
#endif

extern const int nanmask;

/**
*	@return Whether the given float is Not a Number.
*/
inline bool IS_NAN( const float flValue )
{
	return ( ( ( *( int* ) &flValue ) & nanmask ) == nanmask );
}

#endif //PUBLIC_MATH_MATHLIB_H