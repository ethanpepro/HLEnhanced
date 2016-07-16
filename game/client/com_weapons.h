//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// com_weapons.h
// Shared weapons common function prototypes
#if !defined( COM_WEAPONSH )
#define COM_WEAPONSH
#ifdef _WIN32
#pragma once
#endif

#include "Exports.h"

void			COM_Log( char *pszFile, char *fmt, ...);
int				CL_IsDead();

int				HUD_GetWeaponAnim();
void			HUD_SendWeaponAnim( int iAnim, int body, int force );
void			HUD_PlaySound( char *sound, float volume );

extern cvar_t *cl_lw;

extern int g_runfuncs;
extern Vector v_angles;
extern float g_lastFOV;
extern struct local_state_s *g_finalstate;

void CL_SetupServerSupport();

#endif