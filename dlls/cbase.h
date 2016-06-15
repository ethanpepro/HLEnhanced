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
#include "schedule.h"

#ifndef MONSTEREVENT_H
#include "monsterevent.h"
#endif

#include "DataMapping.h"

#include "EHandle.h"

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
extern void	DispatchObjectCollsionBox( edict_t *pent );
extern void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount );
extern void SaveGlobalState( SAVERESTOREDATA *pSaveData );
extern void RestoreGlobalState( SAVERESTOREDATA *pSaveData );
extern void ResetGlobalState( void );

typedef enum { USE_OFF = 0, USE_ON = 1, USE_SET = 2, USE_TOGGLE = 3 } USE_TYPE;

extern void FireTargets( const char *targetName, CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

typedef void (CBaseEntity::*BASEPTR)(void);
typedef void (CBaseEntity::*ENTITYFUNCPTR)(CBaseEntity *pOther );
typedef void (CBaseEntity::*USEPTR)( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

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

#include "CBaseEntity.shared.h"

class CPointEntity : public CBaseEntity
{
public:
	DECLARE_CLASS( CPointEntity, CBaseEntity );

	void	Spawn( void ) override;
	virtual int	ObjectCaps( void ) override { return CBaseEntity :: ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
private:
};


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
	BYTE	bEOFLocked;				// true if hit end of list of locked sentences
	BYTE	bEOFUnlocked;			// true if hit end of list of unlocked sentences
} locksound_t;

void PlayLockSounds( entvars_t *pev, locksound_t *pls, const bool bLocked, const bool bButton );

//
// generic Delay entity.
//
class CBaseDelay : public CBaseEntity
{
public:
	DECLARE_CLASS( CBaseDelay, CBaseEntity );
	DECLARE_DATADESC();

	float		m_flDelay;
	int			m_iszKillTarget;

	virtual void	KeyValue( KeyValueData* pkvd) override;
	// common member functions
	void SUB_UseTargets( CBaseEntity *pActivator, USE_TYPE useType, float value );
	void EXPORT DelayThink( void );
};


class CBaseAnimating : public CBaseDelay
{
public:
	DECLARE_CLASS( CBaseAnimating, CBaseDelay );
	DECLARE_DATADESC();

	// Basic Monster Animation functions
	float StudioFrameAdvance( float flInterval = 0.0 ); // accumulate animation frame time from last time called until now
	int	 GetSequenceFlags() const;
	int  LookupActivity ( int activity );
	int  LookupActivityHeaviest ( int activity );
	int  LookupSequence ( const char *label );
	void ResetSequenceInfo ( );
	void DispatchAnimEvents ( float flFutureInterval = 0.1 ); // Handle events that have happend since last time called up until X seconds into the future
	virtual void HandleAnimEvent( MonsterEvent_t *pEvent ) { return; };
	float SetBoneController ( int iController, float flValue );
	void InitBoneControllers ( void );
	float SetBlending ( int iBlender, float flValue );
	void GetBonePosition ( int iBone, Vector &origin, Vector &angles );
	void GetAutomovement( Vector &origin, Vector &angles, float flInterval = 0.1 );
	int  FindTransition( int iEndingSequence, int iGoalSequence, int *piDir );
	void GetAttachment ( int iAttachment, Vector &origin, Vector &angles );
	void SetBodygroup( int iGroup, int iValue );
	int GetBodygroup( int iGroup );
	int ExtractBbox( int sequence, float *mins, float *maxs );
	void SetSequenceBox( void );

	// animation needs
	float				m_flFrameRate;		// computed FPS for current sequence
	float				m_flGroundSpeed;	// computed linear movement rate for current sequence
	float				m_flLastEventCheck;	// last time the event list was checked
	bool				m_fSequenceFinished;// flag set when StudioAdvanceFrame moves across a frame boundry
	bool				m_fSequenceLoops;	// true if the sequence loops
};


//
// generic Toggle entity.
//
#define	SF_ITEM_USE_ONLY	256 //  ITEM_USE_ONLY = BUTTON_USE_ONLY = DOOR_USE_ONLY!!! 

class CBaseToggle : public CBaseAnimating
{
public:
	DECLARE_CLASS( CBaseToggle, CBaseAnimating );
	DECLARE_DATADESC();

	void				KeyValue( KeyValueData *pkvd ) override;

	TOGGLE_STATE		m_toggle_state;
	float				m_flActivateFinished;//like attack_finished, but for doors
	float				m_flMoveDistance;// how far a door should slide or rotate
	float				m_flWait;
	float				m_flLip;
	float				m_flTWidth;// for plats
	float				m_flTLength;// for plats

	Vector				m_vecPosition1;
	Vector				m_vecPosition2;
	Vector				m_vecAngle1;
	Vector				m_vecAngle2;

	int					m_cTriggersLeft;		// trigger_counter only, # of activations remaining
	float				m_flHeight;
	EHANDLE				m_hActivator;
	void (CBaseToggle::*m_pfnCallWhenMoveDone)(void);
	Vector				m_vecFinalDest;
	Vector				m_vecFinalAngle;

	int					m_bitsDamageInflict;	// DMG_ damage type that the door or tigger does

	virtual int		GetToggleState( void ) override { return m_toggle_state; }
	virtual float	GetDelay( void ) override { return m_flWait; }

	// common member functions
	void LinearMove( Vector	vecDest, float flSpeed );
	void EXPORT LinearMoveDone( void );
	void AngularMove( Vector vecDestAngle, float flSpeed );
	void EXPORT AngularMoveDone( void );
	bool IsLockedByMaster() const override;

	static float		AxisValue( int flags, const Vector &angles );
	static void			AxisDir( entvars_t *pev );
	static float		AxisDelta( int flags, const Vector &angle1, const Vector &angle2 );

	string_t m_sMaster;		// If this button has a master switch, this is the targetname.
							// A master switch must be of the multisource type. If all 
							// of the switches in the multisource have been triggered, then
							// the button will be allowed to operate. Otherwise, it will be
							// deactivated.
};
#define SetMoveDone( a ) m_pfnCallWhenMoveDone = static_cast <void (CBaseToggle::*)(void)> (a)


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

// when calling KILLED(), a value that governs gib behavior is expected to be 
// one of these three values
#define GIB_NORMAL			0// gib if entity was overkilled
#define GIB_NEVER			1// never gib, no matter how much death damage is done ( freezing, etc )
#define GIB_ALWAYS			2// always gib ( Houndeye Shock, Barnacle Bite )

class CBaseMonster;
class CCineMonster;
class CSound;

#include "basemonster.h"


char *ButtonSound( int sound );				// get string of button sound number


//
// Generic Button
//
class CBaseButton : public CBaseToggle
{
public:
	DECLARE_CLASS( CBaseButton, CBaseToggle );
	DECLARE_DATADESC();

	void Spawn( void ) override;
	virtual void Precache( void ) override;
	void RotSpawn( void );
	virtual void KeyValue( KeyValueData* pkvd) override;

	void ButtonActivate( );
	void SparkSoundCache( void );

	void EXPORT ButtonShot( void );
	void EXPORT ButtonTouch( CBaseEntity *pOther );
	void EXPORT ButtonSpark ( void );
	void EXPORT TriggerAndWait( void );
	void EXPORT ButtonReturn( void );
	void EXPORT ButtonBackHome( void );
	void EXPORT ButtonUse ( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual int		TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType ) override;
	
	enum BUTTON_CODE { BUTTON_NOTHING, BUTTON_ACTIVATE, BUTTON_RETURN };
	BUTTON_CODE	ButtonResponseToTouch( void );
	
	// Buttons that don't take damage can be IMPULSE used
	virtual int	ObjectCaps( void ) override { return (CBaseToggle:: ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | (pev->takedamage?0:FCAP_IMPULSE_USE); }

	bool	m_fStayPushed;	// button stays pushed in until touched again?
	bool	m_fRotating;		// a rotating button?  default is a sliding button.

	string_t m_strChangeTarget;	// if this field is not null, this is an index into the engine string array.
							// when this button is touched, it's target entity's TARGET field will be set
							// to the button's ChangeTarget. This allows you to make a func_train switch paths, etc.

	locksound_t m_ls;			// door lock sounds
	
	BYTE	m_bLockedSound;		// ordinals from entity selection
	BYTE	m_bLockedSentence;	
	BYTE	m_bUnlockedSound;	
	BYTE	m_bUnlockedSentence;
	int		m_sounds;
};

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

// this moved here from world.cpp, to allow classes to be derived from it
//=======================
// CWorld
//
// This spawns first when each level begins.
//=======================
class CWorld : public CBaseEntity
{
public:
	DECLARE_CLASS( CWorld, CBaseEntity );

	void OnCreate() override;

	void OnDestroy() override;

	void Spawn( void ) override;
	void Precache( void ) override;
	void KeyValue( KeyValueData *pkvd ) override;

	/**
	*	Gets the global instance of this entity.
	*/
	static CWorld* GetInstance()
	{
		return m_pInstance;
	}

private:
	static CWorld* m_pInstance;
};

#endif //CBASE_H