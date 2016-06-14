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
/*

===== h_export.cpp ========================================================

  Entity classes exported by Halflife.

*/

#include "extdll.h"
#include "util.h"

#include "cbase.h"

// Holds engine functionality callbacks
enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals;

#ifdef _WIN32
//The one function in Half-Life 1 that uses stdcall. Be aware of this. - Solokiller
#define GIVEFNPTRS_DLLEXPORT __stdcall
#else
#define GIVEFNPTRS_DLLEXPORT DLLEXPORT
#endif

extern "C" void GIVEFNPTRS_DLLEXPORT GiveFnptrsToDll( enginefuncs_t* pengfuncsFromEngine, globalvars_t* pGlobals )
{
	memcpy( &g_engfuncs, pengfuncsFromEngine, sizeof( enginefuncs_t ) );
	gpGlobals = pGlobals;
}


