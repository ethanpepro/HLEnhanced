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

#include "mathlib2.h"

#ifndef DID_VEC3_T_DEFINE
#define DID_VEC3_T_DEFINE
typedef vec_t vec3_t[3];
#endif
typedef vec_t vec4_t[4];	// x,y,z,w

struct mplane_s;

#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#define VectorAdd(a,b,c) {(c)[0]=(a)[0]+(b)[0];(c)[1]=(a)[1]+(b)[1];(c)[2]=(a)[2]+(b)[2];}
#define VectorCopy(a,b) {(b)[0]=(a)[0];(b)[1]=(a)[1];(b)[2]=(a)[2];}
#define VectorClear(a) {(a)[0]=0.0;(a)[1]=0.0;(a)[2]=0.0;}

void VectorMA (const vec3_t veca, float scale, const vec3_t vecb, vec3_t vecc);

void CrossProduct (const vec3_t v1, const vec3_t v2, vec3_t cross);
float VectorNormalize (vec3_t v);		// returns vector length
void VectorScale (const vec3_t in, vec_t scale, vec3_t out);
int Q_log2(int val);

// Here are some "manual" INLINE routines for doing floating point to integer conversions
extern short new_cw, old_cw;

typedef union DLONG {
	int		i[2];
	double	d;
	float	f;
	} DLONG;

extern DLONG	dlong;

#ifdef _WIN32
void __inline set_fpu_cw(void)
{
_asm	
	{		wait
			fnstcw	old_cw
			wait
			mov		ax, word ptr old_cw
			or		ah, 0xc
			mov		word ptr new_cw,ax
			fldcw	new_cw
	}
}

int __inline quick_ftol(float f)
{
	_asm {
		// Assumes that we are already in chop mode, and only need a 32-bit int
		fld		DWORD PTR f
		fistp	DWORD PTR dlong
	}
	return dlong.i[0];
}

void __inline restore_fpu_cw(void)
{
	_asm	fldcw	old_cw
}
#else
#define set_fpu_cw() /* */
#define quick_ftol(f) ftol(f)
#define restore_fpu_cw() /* */
#endif

void AngleVectors (const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
void AngleVectorsTranspose (const vec3_t angles, vec3_t forward, vec3_t right, vec3_t up);
#define AngleIVectors	AngleVectorsTranspose

void AngleMatrix (const vec3_t angles, float (*matrix)[4] );
void VectorTransform (const vec3_t in1, float in2[3][4], vec3_t out);

void NormalizeAngles( float* angles );
float AngleBetweenVectors( const vec3_t v1, const vec3_t v2 );


void VectorMatrix( vec3_t forward, vec3_t right, vec3_t up);
void VectorAngles( const vec3_t forward, vec3_t angles );

float	anglemod(float a);



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
