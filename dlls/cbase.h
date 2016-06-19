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

class CBaseEntity;
class CBaseMonster;
class CSquadMonster;
class CCineMonster;
class CBasePlayerItem;
class CSound;

/**
*	Max number of nodes available for a path.
*/
#define MAX_PATH_SIZE 10

/**
*	These are caps bits to indicate what an object's capabilities (currently used for save/restore and level transitions).
*/
enum FCapability
{
	FCAP_CUSTOMSAVE				= 0x00000001,

	/**
	*	Should transfer between transitions.
	*/
	FCAP_ACROSS_TRANSITION		= 0x00000002,

	/**
	*	Spawn after restore.
	*/
	FCAP_MUST_SPAWN				= 0x00000004,

	/**
	*	Don't save this.
	*/
	FCAP_DONT_SAVE				= 0x80000000,

	/**
	*	Can be used by the player.
	*/
	FCAP_IMPULSE_USE			= 0x00000008,

	/**
	*	Can be used by the player.
	*/
	FCAP_CONTINUOUS_USE			= 0x00000010,

	/**
	*	Can be used by the player.
	*/
	FCAP_ONOFF_USE				= 0x00000020,

	/**
	*	Player sends +/- 1 when using (currently only tracktrains).
	*/
	FCAP_DIRECTIONAL_USE		= 0x00000040,

	/**
	*	Can be used to "master" other entities (like multisource).
	*/
	FCAP_MASTER					= 0x00000080,

	/**
	*	UNDONE: This will ignore transition volumes (trigger_transition), but not the PVS!!!
	*	ALWAYS goes across transitions.
	*/
	FCAP_FORCE_TRANSITION		= 0x00000080,
};

#include "archtypes.h"     // DAL
#include "SaveRestore.h"
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

enum USE_TYPE
{
	USE_OFF = 0,
	USE_ON = 1,
	USE_SET = 2,
	USE_TOGGLE = 3
};

extern void FireTargets( const char *targetName, CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

/**
*	@see CBaseEntity::Classify
*/
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

	/**
	*	Hornets and snarks. Launched by players.
	*/
	CLASS_PLAYER_BIOWEAPON	 = 12,

	/**
	*	Hornets and snarks. Launched by the alien menace.
	*/
	CLASS_ALIEN_BIOWEAPON	 = 13,

	/**
	*	TODO: entities that want to be ignored use this. Rename. - Solokiller
	*	Special because no one pays attention to it, and it eats a wide cross-section of creatures.
	*/
	CLASS_BARNACLE			 = 99,
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

/**
*	Set this bit on guns and stuff that should never respawn.
*/
#define	SF_NORESPAWN	( 1 << 30 )

/**
*	People gib if their health is <= this at the time of death.
*/
#define	GIB_HEALTH_VALUE	-30

// NOTE: tweak these values based on gameplay feedback:

/**
*	Number of 2 second intervals to take damage.
*/
#define PARALYZE_DURATION	2

/**
*	Damage to take each 2 second interval.
*/
#define PARALYZE_DAMAGE		1.0

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

/**
*	Tracers fire every 4 bullets
*/
#define TRACER_FREQ 4

#include "Damage.h"

#include "ButtonSounds.h"

#include "entities/CBaseEntity.shared.h"

#include "entities/CPointEntity.h"

#include "entities/CBaseDelay.h"

#include "entities/CBaseAnimating.h"

#include "entities/CBaseToggle.h"

#include "entities/NPCs/CBaseMonster.h"

#include "entities/CBaseButton.h"

/**
*	Converts a entvars_t * to a class pointer
*	It will allocate the class and entity if necessary
*/
template<typename T>
T* GetClassPtr( T* a )
{
	entvars_t* pev = reinterpret_cast<entvars_t*>( a );

	// allocate entity if necessary
	if( pev == nullptr )
		pev = VARS( CREATE_ENTITY() );

	// get the private data
	a = static_cast<T*>( GET_PRIVATE( ENT( pev ) ) );

	if( a == nullptr ) 
	{
		// allocate private data 
		a = new( pev ) T;
		a->pev = pev;
		//Now calls OnCreate - Solokiller
		a->OnCreate();
	}

	return a;
}

#include "entities/CWorld.h"

#endif //CBASE_H