//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// eventscripts.h
#if !defined ( EVENTSCRIPTSH )
#define EVENTSCRIPTSH

#include "event_args.h"

// defaults for clientinfo messages
#define	DEFAULT_VIEWHEIGHT	28
#define VEC_DUCK_VIEW 12

#define FTENT_FADEOUT			0x00000080

#include "Damage.h"

// Some of these are HL/TFC specific?
void EV_EjectBrass( float *origin, float *velocity, float rotation, int model, int soundtype );
void EV_GetGunPosition( event_args_t *args, float *pos, float *origin );
void EV_GetDefaultShellInfo( event_args_t *args, 
							 float *origin, float *velocity, 
							 float *ShellVelocity, float *ShellOrigin, 
							 float *forward, float *right, float *up, 
							 float forwardScale, float upScale, float rightScale );
qboolean EV_IsLocal( int idx );
qboolean EV_IsPlayer( int idx );
void EV_CreateTracer( float *start, float *end );

struct cl_entity_s *GetEntity( int idx );
struct cl_entity_s *GetViewEntity( void );
void EV_MuzzleFlash( void );

#endif // EVENTSCRIPTSH
