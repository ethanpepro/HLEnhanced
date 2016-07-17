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
#include "archtypes.h"     // DAL

//
// Misc utility code
//
#include "entities/NPCs/Activity.h"

#ifndef ENGINECALLBACK_H
#include "enginecallback.h"
#endif

#include "mathlib.h"

#include "shared_game_utils.h"

extern globalvars_t				*gpGlobals;

// Use this instead of ALLOC_STRING on constant strings
#define STRING(offset)		((const char *)(gpGlobals->pStringBase + (unsigned int)(offset)))
#define MAKE_STRING(str)	((uint64)(str) - (uint64)(STRING(0)))

inline edict_t *FIND_ENTITY_BY_CLASSNAME(edict_t *entStart, const char *pszName) 
{
	return FIND_ENTITY_BY_STRING(entStart, "classname", pszName);
}	

inline edict_t *FIND_ENTITY_BY_TARGETNAME(edict_t *entStart, const char *pszName) 
{
	return FIND_ENTITY_BY_STRING(entStart, "targetname", pszName);
}	

// for doing a reverse lookup. Say you have a door, and want to find its button.
inline edict_t *FIND_ENTITY_BY_TARGET(edict_t *entStart, const char *pszName) 
{
	return FIND_ENTITY_BY_STRING(entStart, "target", pszName);
}	

// Keeps clutter down a bit, when using a float as a bit-vector
#define SetBits(flBitVector, bits)		((flBitVector) = (int)(flBitVector) | (bits))
#define ClearBits(flBitVector, bits)	((flBitVector) = (int)(flBitVector) & ~(bits))
#define FBitSet(flBitVector, bit)		((int)(flBitVector) & (bit))

// In case this ever changes
#define M_PI			3.14159265358979323846

// This is the glue that hooks .MAP entity class names to our CPP classes
// The _declspec forces them to be exported by name so we can do a lookup with GetProcAddress()
// The function is used to intialize / allocate the object for the entity
#define LINK_ENTITY_TO_CLASS(mapClassName,DLLClassName) \
	extern "C" DLLEXPORT void mapClassName( entvars_t *pev ); \
	void mapClassName( entvars_t *pev ) { GetClassPtr( (DLLClassName *)pev ); }

//
// Conversion among the three types of "entity", including identity-conversions.
//
#ifdef DEBUG
	extern edict_t *DBG_EntOfVars(const entvars_t *pev);
	inline edict_t *ENT(const entvars_t *pev)	{ return DBG_EntOfVars(pev); }
#else
	inline edict_t *ENT(const entvars_t *pev)	{ return pev->pContainingEntity; }
#endif
inline edict_t *ENT(edict_t *pent)		{ return pent; }
inline edict_t *ENT(EOFFSET eoffset)			{ return (*g_engfuncs.pfnPEntityOfEntOffset)(eoffset); }
edict_t* ENT( CBaseEntity* pEntity );
inline EOFFSET OFFSET(EOFFSET eoffset)			{ return eoffset; }
inline EOFFSET OFFSET(const edict_t *pent)	
{ 
#if _DEBUG
	if ( !pent )
		ALERT( at_error, "Bad ent in OFFSET()\n" );
#endif
	return (*g_engfuncs.pfnEntOffsetOfPEntity)(pent); 
}
inline EOFFSET OFFSET( const entvars_t *pev )				
{ 
#if _DEBUG
	if ( !pev )
		ALERT( at_error, "Bad pev in OFFSET()\n" );
#endif
	return OFFSET( ENT( pev ) ); 
}
inline entvars_t *VARS(entvars_t *pev)					{ return pev; }

inline entvars_t *VARS(edict_t *pent)			
{ 
	if ( !pent )
		return NULL;

	return &pent->v; 
}

inline entvars_t* VARS(EOFFSET eoffset)				{ return VARS(ENT(eoffset)); }
entvars_t* VARS( CBaseEntity* pEntity );
inline int ENTINDEX( const edict_t *pEdict )			{ return (*g_engfuncs.pfnIndexOfEdict)(pEdict); }
inline edict_t* INDEXENT( int iEdictNum )		{ return (*g_engfuncs.pfnPEntityOfEntIndex)(iEdictNum); }
inline void MESSAGE_BEGIN( int iMsgType, int iMsgID, const float *pOrigin, entvars_t *ent ) {
	(*g_engfuncs.pfnMessageBegin)( iMsgType, iMsgID, pOrigin, ENT(ent));
}

// Testing the three types of "entity" for nullity
#define eoNullEntity 0
inline bool FNullEnt(EOFFSET eoffset)		{ return eoffset == eoNullEntity; }
inline bool FNullEnt(const edict_t* pent)	{ return pent == NULL || FNullEnt(OFFSET(pent)); }
inline bool FNullEnt(entvars_t* pev)		{ return pev == NULL || FNullEnt(OFFSET(pev)); }
bool FNullEnt( const CBaseEntity* pEntity );

// Testing strings for nullity
#define iStringNull 0
inline bool FStringNull(string_t iString)			{ return iString == iStringNull; }

#define cchMapNameMost 32

// Dot products for view cone checking
#define VIEW_FIELD_FULL		(float)-1.0 // +-180 degrees
#define	VIEW_FIELD_WIDE		(float)-0.7 // +-135 degrees 0.1 // +-85 degrees, used for full FOV checks 
#define	VIEW_FIELD_NARROW	(float)0.7 // +-45 degrees, more narrow check used to set up ranged attacks
#define	VIEW_FIELD_ULTRA_NARROW	(float)0.9 // +-25 degrees, more narrow check used to set up ranged attacks

// All monsters need this data
enum BloodColor
{
	DONT_BLEED			 = -1,
	BLOOD_COLOR_RED		 = 247,
	BLOOD_COLOR_YELLOW	 = 195,
	BLOOD_COLOR_GREEN	 = BLOOD_COLOR_YELLOW,
};

enum MONSTERSTATE
{
	MONSTERSTATE_NONE = 0,
	MONSTERSTATE_IDLE,
	MONSTERSTATE_COMBAT,
	MONSTERSTATE_ALERT,
	MONSTERSTATE_HUNT,
	MONSTERSTATE_PRONE,
	MONSTERSTATE_SCRIPT,
	MONSTERSTATE_PLAYDEAD,
	MONSTERSTATE_DEAD
};

// Things that toggle (buttons/triggers/doors) need this
enum TOGGLE_STATE
{
	TS_AT_TOP,
	TS_AT_BOTTOM,
	TS_GOING_UP,
	TS_GOING_DOWN
};

// Misc useful
inline bool FStrEq(const char*sz1, const char*sz2)
	{ return (strcmp(sz1, sz2) == 0); }
inline bool FClassnameIs(edict_t* pent, const char* szClassname)
	{ return FStrEq(STRING(VARS(pent)->classname), szClassname); }
inline bool FClassnameIs(entvars_t* pev, const char* szClassname)
	{ return FStrEq(STRING(pev->classname), szClassname); }

class CBaseEntity;
class CBasePlayerItem;
class CBasePlayer;

// Misc. Prototypes
void UTIL_SetSize( CBaseEntity* pEntity, const Vector& vecMin, const Vector& vecMax );
extern float		UTIL_VecToYaw			(const Vector &vec);

extern CBaseEntity	*UTIL_FindEntityInSphere(CBaseEntity *pStartEntity, const Vector &vecCenter, float flRadius);
extern CBaseEntity	*UTIL_FindEntityByString(CBaseEntity *pStartEntity, const char *szKeyword, const char *szValue );
extern CBaseEntity	*UTIL_FindEntityByClassname(CBaseEntity *pStartEntity, const char *szName );
extern CBaseEntity	*UTIL_FindEntityByTargetname(CBaseEntity *pStartEntity, const char *szName );
extern CBaseEntity	*UTIL_FindEntityGeneric(const char *szName, Vector &vecSrc, float flRadius );

// returns a CBasePlayer pointer to a player by index.  Only returns if the player is spawned and connected
// otherwise returns nullptr
// Index is 1 based
// Now returns CBasePlayer - Solokiller
extern CBasePlayer	*UTIL_PlayerByIndex( int playerIndex );

/**
*	Find a player with a case-insensitive name search.
*	@param pszTestName Player name.
*	@return Player instance, or null if no player exists by that name.
*/
CBasePlayer* UTIL_FindPlayerByName( const char* pszTestName );

#define UTIL_EntitiesInPVS(pent)			(*g_engfuncs.pfnEntitiesInPVS)(pent)
extern void			UTIL_MakeVectors		(const Vector &vecAngles);

// Pass in an array of pointers and an array size, it fills the array and returns the number inserted
extern int			UTIL_MonstersInSphere( CBaseEntity **pList, int listMax, const Vector &center, float radius );
extern int			UTIL_EntitiesInBox( CBaseEntity **pList, int listMax, const Vector &mins, const Vector &maxs, int flagMask );

inline void UTIL_MakeVectorsPrivate( const Vector &vecAngles, float *p_vForward, float *p_vRight, float *p_vUp )
{
	g_engfuncs.pfnAngleVectors( vecAngles, p_vForward, p_vRight, p_vUp );
}

extern void			UTIL_MakeAimVectors		( const Vector &vecAngles ); // like MakeVectors, but assumes pitch isn't inverted
extern void			UTIL_MakeInvVectors		( const Vector &vec, globalvars_t *pgv );

void UTIL_SetOrigin( CBaseEntity* pEntity, const Vector& vecOrigin );
extern void			UTIL_ParticleEffect		( const Vector &vecOrigin, const Vector &vecDirection, const unsigned int ulColor, const unsigned int ulCount );
extern void			UTIL_ScreenShake		( const Vector &center, float amplitude, float frequency, float duration, float radius );
extern void			UTIL_ScreenShakeAll		( const Vector &center, float amplitude, float frequency, float duration );
extern void			UTIL_ShowMessage		( const char *pString, CBaseEntity *pPlayer );
extern void			UTIL_ShowMessageAll		( const char *pString );
extern void			UTIL_ScreenFadeAll		( const Vector &color, float fadeTime, float holdTime, int alpha, int flags );
extern void			UTIL_ScreenFade			( CBaseEntity *pEntity, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags );

typedef enum { ignore_monsters=1, dont_ignore_monsters=0, missile=2 } IGNORE_MONSTERS;
typedef enum { ignore_glass=1, dont_ignore_glass=0 } IGNORE_GLASS;
extern void			UTIL_TraceLine			(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr);
extern void			UTIL_TraceLine			(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr);
extern void			UTIL_TraceHull			(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, const Hull::Hull hullNumber, edict_t *pentIgnore, TraceResult *ptr);
extern TraceResult	UTIL_GetGlobalTrace		(void);
extern void			UTIL_TraceModel			(const Vector &vecStart, const Vector &vecEnd, const Hull::Hull hullNumber, edict_t *pentModel, TraceResult *ptr);
extern Vector		UTIL_GetAimVector		(edict_t* pent, float flSpeed);

extern bool			UTIL_IsMasterTriggered( string_t sMaster, const CBaseEntity* const pActivator );
extern void			UTIL_BloodStream( const Vector &origin, const Vector &direction, int color, int amount );
extern void			UTIL_BloodDrips( const Vector &origin, const Vector &direction, int color, int amount );
extern Vector		UTIL_RandomBloodVector( void );
extern bool			UTIL_ShouldShowBlood( int bloodColor );
extern void			UTIL_BloodDecalTrace( TraceResult *pTrace, int bloodColor );
extern void			UTIL_DecalTrace( TraceResult *pTrace, int decalNumber );
extern void			UTIL_PlayerDecalTrace( TraceResult *pTrace, int playernum, int decalNumber, const bool bIsCustom );

/**
*	Projects a gunshot decal.
*	@param pTrace Trace to use when projecting. The decal is projected onto the entity it has hit.
*	@param decalNumber Decal index to use.
*/
extern void			UTIL_GunshotDecalTrace( TraceResult *pTrace, int decalNumber );
extern void			UTIL_Sparks( const Vector &position );
void DoSpark( CBaseEntity* pEntity, const Vector &location );
extern void			UTIL_Ricochet( const Vector &position, float scale );

extern void			UTIL_Remove( CBaseEntity *pEntity );
extern bool			UTIL_IsValidEntity( edict_t *pent );
extern bool			UTIL_TeamsMatch( const char *pTeamName1, const char *pTeamName2 );

// allows precacheing of other entities
extern void			UTIL_PrecacheOther( const char *szClassname );

// prints a message to each client
extern void			UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );
inline void			UTIL_CenterPrintAll( const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL ) 
{
	UTIL_ClientPrintAll( HUD_PRINTCENTER, msg_name, param1, param2, param3, param4 );
}

extern bool UTIL_GetNextBestWeapon( CBasePlayer *pPlayer, CBasePlayerItem *pCurrentWeapon );

// prints messages through the HUD
extern void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );

// prints a message to the HUD say (chat)
extern void			UTIL_SayText( const char *pText, CBaseEntity *pEntity );
extern void			UTIL_SayTextAll( const char *pText, CBaseEntity *pEntity );

struct hudtextparms_t
{
	float		x;
	float		y;
	int			effect;
	byte		r1, g1, b1, a1;
	byte		r2, g2, b2, a2;
	float		fadeinTime;
	float		fadeoutTime;
	float		holdTime;
	float		fxTime;
	int			channel;
};

// prints as transparent 'title' to the HUD
extern void			UTIL_HudMessageAll( const hudtextparms_t &textparms, const char *pMessage );
extern void			UTIL_HudMessage( CBaseEntity *pEntity, const hudtextparms_t &textparms, const char *pMessage );

// Writes message to console with timestamp and FragLog header.
extern void			UTIL_LogPrintf( char *fmt, ... );

// Misc functions
extern void SetMovedir( CBaseEntity* pEntity );
extern Vector VecBModelOrigin( const CBaseEntity* const pBModel );
extern int BuildChangeList( LEVELLIST *pLevelList, int maxList );

//
// Constants that were used only by QC (maybe not used at all now)
//
// Un-comment only as needed
//
#define LANGUAGE_ENGLISH				0
#define LANGUAGE_GERMAN					1
#define LANGUAGE_FRENCH					2
#define LANGUAGE_BRITISH				3

extern DLL_GLOBAL int			g_Language;

#define SND_SPAWNING		(1<<8)		// duplicated in protocol.h we're spawing, used in some cases for ambients 
#define SND_STOP			(1<<5)		// duplicated in protocol.h stop sound
#define SND_CHANGE_VOL		(1<<6)		// duplicated in protocol.h change sound vol
#define SND_CHANGE_PITCH	(1<<7)		// duplicated in protocol.h change sound pitch

// func_rotating
#define SF_BRUSH_ROTATE_Y_AXIS		0
#define SF_BRUSH_ROTATE_INSTANT		1
#define SF_BRUSH_ROTATE_BACKWARDS	2
#define SF_BRUSH_ROTATE_Z_AXIS		4
#define SF_BRUSH_ROTATE_X_AXIS		8
#define SF_PENDULUM_AUTO_RETURN		16
#define	SF_PENDULUM_PASSABLE		32


#define SF_BRUSH_ROTATE_SMALLRADIUS	128
#define SF_BRUSH_ROTATE_MEDIUMRADIUS 256
#define SF_BRUSH_ROTATE_LARGERADIUS 512

#define PUSH_BLOCK_ONLY_X	1
#define PUSH_BLOCK_ONLY_Y	2

#define VEC_HULL_MIN		Vector(-16, -16, -36)
#define VEC_HULL_MAX		Vector( 16,  16,  36)
#define VEC_HUMAN_HULL_MIN	Vector( -16, -16, 0 )
#define VEC_HUMAN_HULL_MAX	Vector( 16, 16, 72 )
#define VEC_HUMAN_HULL_DUCK	Vector( 16, 16, 36 )

#define VEC_VIEW			Vector( 0, 0, 28 )

#define VEC_DUCK_HULL_MIN	Vector(-16, -16, -18 )
#define VEC_DUCK_HULL_MAX	Vector( 16,  16,  18)
#define VEC_DUCK_VIEW		Vector( 0, 0, 12 )

#define SVC_TEMPENTITY		23
#define SVC_INTERMISSION	30
#define SVC_CDTRACK			32
#define SVC_WEAPONANIM		35
#define SVC_ROOMTYPE		37
#define	SVC_DIRECTOR		51

#define SPAWNFLAG_NOMESSAGE	1
#define SPAWNFLAG_NOTOUCH	1
#define SPAWNFLAG_DROIDONLY	4

#define SPAWNFLAG_USEONLY	1		// can't be touched, must be used (buttons)

#define TELE_PLAYER_ONLY	1
#define TELE_SILENT			2

// Sound Utilities

#include "sound/Sound.h"

#define PRECACHE_SOUND_ARRAY( a ) \
	{ for (int i = 0; i < ARRAYSIZE( a ); i++ ) PRECACHE_SOUND((char *) a [i]); }

#define EMIT_SOUND_ARRAY_DYN( chan, array ) \
	EMIT_SOUND_DYN ( ENT(pev), chan , array [ RANDOM_LONG(0,ARRAYSIZE( array )-1) ], 1.0, ATTN_NORM, 0, RANDOM_LONG(95,105) ); 

#define RANDOM_SOUND_ARRAY( array ) (array) [ RANDOM_LONG(0,ARRAYSIZE( (array) )-1) ]

#define PLAYBACK_EVENT( flags, who, index ) PLAYBACK_EVENT_FULL( flags, who, index, 0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );
#define PLAYBACK_EVENT_DELAY( flags, who, index, delay ) PLAYBACK_EVENT_FULL( flags, who, index, delay, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );

enum GroupTraceOp
{
	GROUP_OP_AND	= 0,
	GROUP_OP_NAND	= 1
};

extern int g_groupmask;
extern int g_groupop;

class UTIL_GroupTrace
{
public:
	UTIL_GroupTrace( int groupmask, int op );
	~UTIL_GroupTrace( void );

private:
	int m_oldgroupmask, m_oldgroupop;
};

void UTIL_SetGroupTrace( int groupmask, int op );
void UTIL_UnsetGroupTrace( void );

float UTIL_WeaponTimeBase( void );

/**
*	From the set of entities named pszName, randomly selects one entity.
*	@param pszName Entity targetname.
*	@return Randomly selected entity, or null if no entities by that name exist.
*/
CBaseEntity* UTIL_RandomTargetname( const char* const pszName );

/**
*	From the set of entities of class pszName, randomly selects one entity.
*	@param pszName Entity classname.
*	@return Randomly selected entity, or null if no entities by that name exist.
*/
CBaseEntity* UTIL_RandomClassname( const char* pszName );

/**
*	Sets an entvars_t keyvalue, if the key can be found in the datamap.
*/
void EntvarsKeyvalue( entvars_t *pev, KeyValueData *pkvd );

void SetObjectCollisionBox( entvars_t *pev );