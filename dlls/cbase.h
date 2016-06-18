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
#ifndef CBASE_H
#define CBASE_H
/*

Class Hierachy

CBaseEntity
	CBaseDelay
		CBaseToggle
			CBaseItem
			CBaseMonster
				CBaseCycler
				CBasePlayer
*/

#define		MAX_PATH_SIZE	10 // max number of nodes available for a path.

enum FCapability
{
// These are caps bits to indicate what an object's capabilities (currently used for save/restore and level transitions)
	FCAP_CUSTOMSAVE				= 0x00000001,
	FCAP_ACROSS_TRANSITION		= 0x00000002,		// should transfer between transitions
	FCAP_MUST_SPAWN				= 0x00000004,		// Spawn after restore
	FCAP_DONT_SAVE				= 0x80000000,		// Don't save this
	FCAP_IMPULSE_USE			= 0x00000008,		// can be used by the player
	FCAP_CONTINUOUS_USE			= 0x00000010,		// can be used by the player
	FCAP_ONOFF_USE				= 0x00000020,		// can be used by the player
	FCAP_DIRECTIONAL_USE		= 0x00000040,		// Player sends +/- 1 when using (currently only tracktrains)
	FCAP_MASTER					= 0x00000080,		// Can be used to "master" other entities (like multisource)

// UNDONE: This will ignore transition volumes (trigger_transition), but not the PVS!!!
	FCAP_FORCE_TRANSITION		= 0x00000080,		// ALWAYS goes across transitions
};

#include "archtypes.h"     // DAL
#include "saverestore.h"
#include "entities/NPCs/Schedule.h"

#include "entities/AnimationEvent.h"

#include "entities/DataMapping.h"

#include "entities/EHandle.h"

// C functions for external declarations that call the appropriate C++ methods

//TODO: what's with all of the dllexport definitions? - Solokiller
#define EXPORT DLLEXPORT

extern "C" DLLEXPORT int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion );
extern "C" DLLEXPORT int GetEntityAPI2( DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion );

/**
*	Provides new game interfaces to the engine. Not included in the HL SDK, though Ricochet uses it.
*	This is needed in order to trigger the CBaseEntity destructor, as well as provide several other functions.
*	- Solokiller
*/
extern "C" DLLEXPORT int GetNewDLLFunctions( NEW_DLL_FUNCTIONS* pFunctionTable, int* pInterfaceVersion );

extern int DispatchSpawn( edict_t *pent );
extern void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd );
extern void DispatchTouch( edict_t *pentTouched, edict_t *pentOther );
extern void DispatchUse( edict_t *pentUsed, edict_t *pentOther );
extern void DispatchThink( edict_t *pent );
extern void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther );
extern void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData );
extern int  DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity );
extern void	DispatchObjectCollisionBox( edict_t *pent );
extern void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void SaveGlobalState( SAVERESTOREDATA *pSaveData );
extern void RestoreGlobalState( SAVERESTOREDATA *pSaveData );
extern void ResetGlobalState( void );

typedef enum { USE_OFF = 0, USE_ON = 1, USE_SET = 2, USE_TOGGLE = 3 } USE_TYPE;

extern void FireTargets( const char *targetName, CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

// For CLASSIFY
enum Classification
{
	CLASS_NONE				 = 0,
	CLASS_MACHINE			 = 1,
	CLASS_PLAYER			 = 2,
	CLASS_HUMAN_PASSIVE		 = 3,
	CLASS_HUMAN_MILITARY	 = 4,
	CLASS_ALIEN_MILITARY	 = 5,
	CLASS_ALIEN_PASSIVE		 = 6,
	CLASS_ALIEN_MONSTER		 = 7,
	CLASS_ALIEN_PREY		 = 8,
	CLASS_ALIEN_PREDATOR	 = 9,
	CLASS_INSECT			 = 10,
	CLASS_PLAYER_ALLY		 = 11,
	CLASS_PLAYER_BIOWEAPON	 = 12, // hornets and snarks.launched by players
	CLASS_ALIEN_BIOWEAPON	 = 13, // hornets and snarks.launched by the alien menace
	CLASS_BARNACLE			 = 99, //TODO: entities that want to be ignored use this. Rename. - Solokiller// special because no one pays attention to it, and it eats a wide cross-section of creatures.
};

/**
*	Possible values for entvars_t::fixangle
*
*	Solokiller
*/
enum FixAngleMode
{
	/**
	*	Do nothing
	*/
	FIXANGLE_NO			= 0,

	/**
	*	Set view angles to pev->angles
	*/
	FIXANGLE_SET		= 1,

	/**
	*	Add avelocity yaw value to view angles
	*/
	FIXANGLE_ADD_AVEL	= 2
};

class CBaseEntity;
class CBaseMonster;
class CBasePlayerItem;
class CSquadMonster;


#define	SF_NORESPAWN	( 1 << 30 )// !!!set this bit on guns and stuff that should never respawn.

#include "entities/CBaseEntity.shared.h"

#include "entities/CPointEntity.h"

typedef struct locksounds			// sounds that doors and buttons make when locked/unlocked
{
	string_t	sLockedSound;		// sound a door makes when it's locked
	string_t	sLockedSentence;	// sentence group played when door is locked
	string_t	sUnlockedSound;		// sound a door makes when it's unlocked
	string_t	sUnlockedSentence;	// sentence group played when door is unlocked

	int		iLockedSentence;		// which sentence in sentence group to play next
	int		iUnlockedSentence;		// which sentence in sentence group to play next

	float	flwaitSound;			// time delay between playing consecutive 'locked/unlocked' sounds
	float	flwaitSentence;			// time delay between playing consecutive sentences
	bool	bEOFLocked;				// true if hit end of list of locked sentences
	bool	bEOFUnlocked;			// true if hit end of list of unlocked sentences
} locksound_t;

void PlayLockSounds( entvars_t *pev, locksound_t *pls, const bool bLocked, const bool bButton );

#include "entities/CBaseDelay.h"

#include "entities/CBaseAnimating.h"

#include "entities/CBaseToggle.h"

// people gib if their health is <= this at the time of death
#define	GIB_HEALTH_VALUE	-30

#define	ROUTE_SIZE			8 // how many waypoints a monster can store at one time
#define MAX_OLD_ENEMIES		4 // how many old enemies to remember

#define	bits_CAP_DUCK			( 1 << 0 )// crouch
#define	bits_CAP_JUMP			( 1 << 1 )// jump/leap
#define bits_CAP_STRAFE			( 1 << 2 )// strafe ( walk/run sideways)
#define bits_CAP_SQUAD			( 1 << 3 )// can form squads
#define	bits_CAP_SWIM			( 1 << 4 )// proficiently navigate in water
#define bits_CAP_CLIMB			( 1 << 5 )// climb ladders/ropes
#define bits_CAP_USE			( 1 << 6 )// open doors/push buttons/pull levers
#define bits_CAP_HEAR			( 1 << 7 )// can hear forced sounds
#define bits_CAP_AUTO_DOORS		( 1 << 8 )// can trigger auto doors
#define bits_CAP_OPEN_DOORS		( 1 << 9 )// can open manual doors
#define bits_CAP_TURN_HEAD		( 1 << 10)// can turn head, always bone controller 0

#define bits_CAP_RANGE_ATTACK1	( 1 << 11)// can do a range attack 1
#define bits_CAP_RANGE_ATTACK2	( 1 << 12)// can do a range attack 2
#define bits_CAP_MELEE_ATTACK1	( 1 << 13)// can do a melee attack 1
#define bits_CAP_MELEE_ATTACK2	( 1 << 14)// can do a melee attack 2

#define bits_CAP_FLY			( 1 << 15)// can fly, move all around

#define bits_CAP_DOORS_GROUP    (bits_CAP_USE | bits_CAP_AUTO_DOORS | bits_CAP_OPEN_DOORS)

#include "Damage.h"

// NOTE: tweak these values based on gameplay feedback:

#define PARALYZE_DURATION	2		// number of 2 second intervals to take damage
#define PARALYZE_DAMAGE		1.0		// damage to take each 2 second interval

#define NERVEGAS_DURATION	2
#define NERVEGAS_DAMAGE		5.0

#define POISON_DURATION		5
#define POISON_DAMAGE		2.0

#define RADIATION_DURATION	2
#define RADIATION_DAMAGE	1.0

#define ACID_DURATION		2
#define ACID_DAMAGE			5.0

#define SLOWBURN_DURATION	2
#define SLOWBURN_DAMAGE		1.0

#define SLOWFREEZE_DURATION	2
#define SLOWFREEZE_DAMAGE	1.0


#define	itbd_Paralyze		0		
#define	itbd_NerveGas		1
#define	itbd_Poison			2
#define	itbd_Radiation		3
#define	itbd_DrownRecover	4
#define	itbd_Acid			5
#define	itbd_SlowBurn		6
#define	itbd_SlowFreeze		7
#define CDMG_TIMEBASED		8

class CBaseMonster;
class CCineMonster;
class CSound;

#include "entities/NPCs/CBaseMonster.h"

#include "ButtonSounds.h"

#include "entities/CBaseButton.h"

//
// Converts a entvars_t * to a class pointer
// It will allocate the class and entity if necessary
//
template <class T> T * GetClassPtr( T *a )
{
	entvars_t *pev = (entvars_t *)a;

	// allocate entity if necessary
	if (pev == NULL)
		pev = VARS(CREATE_ENTITY());

	// get the private data
	a = (T *)GET_PRIVATE(ENT(pev));

	if (a == NULL) 
	{
		// allocate private data 
		a = new(pev) T;
		a->pev = pev;
		//Now calls OnCreate - Solokiller
		a->OnCreate();
	}
	return a;
}


/*
bit_PUSHBRUSH_DATA | bit_TOGGLE_DATA
bit_MONSTER_DATA
bit_DELAY_DATA
bit_TOGGLE_DATA | bit_DELAY_DATA | bit_MONSTER_DATA
bit_PLAYER_DATA | bit_MONSTER_DATA
bit_MONSTER_DATA | CYCLER_DATA
bit_LIGHT_DATA
path_corner_data
bit_MONSTER_DATA | wildcard_data
bit_MONSTER_DATA | bit_GROUP_DATA
boid_flock_data
boid_data
CYCLER_DATA
bit_ITEM_DATA
bit_ITEM_DATA | func_hud_data
bit_TOGGLE_DATA | bit_ITEM_DATA
EOFFSET
env_sound_data
env_sound_data
push_trigger_data
*/

#define TRACER_FREQ		4			// Tracers fire every 4 bullets

#include "entities/CWorld.h"

#endif //CBASE_H