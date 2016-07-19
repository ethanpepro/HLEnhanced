#ifndef GAME_SHARED_SHARED_GAME_UTILS_H
#define GAME_SHARED_SHARED_GAME_UTILS_H

#include "FileSystem.h"

#include "CHashStringPool.h"

/**
*	dll agnostic game utility functionality - Solokiller
*/

// Makes these more explicit, and easier to find
#define FILE_GLOBAL static
#define DLL_GLOBAL

// Until we figure out why "const" gives the compiler problems, we'll just have to use
// this bogus "empty" define to mark things as constant.
#define CONSTANT

#define BUBBLES_MODEL "sprites/bubble.spr"

/**
*	Number of static buffers used by functions that return pointers to static string buffers.
*/
const size_t NUM_STATIC_BUFFERS = 4;

extern DLL_GLOBAL const Vector g_vecZero;

extern cvar_t* g_pDeveloper;

/**
*	Global filesystem instance.
*/
extern IFileSystem* g_pFileSystem;

class CBaseEntity;

/**
*	Gets the CBaseEntity instance out of the edict, if it's a valid instance.
*	This used to return void*. This led to unsafe casts that could cause invalid conversions (and has, in some cases). Use static_cast or dynamic_cast. - Solokiller
*/
CBaseEntity* GET_PRIVATE( edict_t* pent );

extern CHashStringPool g_StringPool;

int UTIL_SharedRandomLong( unsigned int seed, int low, int high );
float UTIL_SharedRandomFloat( unsigned int seed, float low, float high );

void UTIL_StringToVector( float *pVector, const char *pString );

void UTIL_StringToIntArray( int *pVector, int count, const char *pString );

/**
*	Utility function to format strings without creating a buffer to store the result in.
*	@param pszFormat Format string.
*	@param ... Arguments.
*	@return Pointer to the string. Up to NUM_STATIC_BUFFERS strings returned sequentially from this can be valid at the same time.
*	@see NUM_STATIC_BUFFERS.
*/
char* UTIL_VarArgs( const char* pszFormat, ... );

/**
*	For handy use with ClientPrint params. This returns the string representation of the given integer.
*	@param iValue Value.
*	@return Pointer to the string. Up to NUM_STATIC_BUFFERS strings returned sequentially from this can be valid at the same time.
*	@see NUM_STATIC_BUFFERS.
*/
char* UTIL_dtos( const int iValue );

void UTIL_StripToken( const char *pKey, char *pDest );// for redundant keynames

char *safe_strcpy( char *dst, const char *src, const size_t len_dst );

int safe_sprintf( char *dst, const size_t len_dst, const char *format, ... );

//
// How did I ever live without ASSERT?
//
#ifdef	DEBUG
void DBG_AssertFunction( const bool fExpr, const char* szExpr, const char* szFile, int szLine, const char* szMessage );
#define ASSERT(f)		DBG_AssertFunction( !!( f ), #f, __FILE__, __LINE__, NULL )
#define ASSERTSZ(f, sz)	DBG_AssertFunction( !!( f ), #f, __FILE__, __LINE__, sz )
#else	// !DEBUG
#define ASSERT(f)
#define ASSERTSZ(f, sz)
#endif	// !DEBUG

#define MAX_COM_TOKEN 1500

extern char com_token[ MAX_COM_TOKEN ];

//TODO: tidy the parse code, make it use user provided buffers - Solokiller
char *COM_Parse( char *data );

int COM_TokenWaiting( char *buffer );

char *memfgets( byte *pMemFile, int fileSize, int &filePos, char *pBuffer, int bufferSize );

/**
*	Cross-dll get cvar pointer function.
*	@param pszName Name of the cvar.
*	@return CVar, or null if the cvar doesn't exist.
*/
cvar_t* CVarGetPointer( const char* const pszName );

/**
*	Cross-dll alert function.
*	@param aType Alert type.
*	@param pszFormat Format string.
*	@param ... Arguments.
*/
void Alert( ALERT_TYPE aType, const char* const pszFormat, ... );

/**
*	Gets the contents for the given position.
*	@param vec Position.
*	@return Contents.
*	@see Contents
*/
Contents UTIL_PointContents( const Vector& vec );

/**
*	Search for water transition along a vertical line.
*	@param vecPosition Position.
*	@param minz Minimum height level.
*	@param maxz Maximum height level.
*	@return Water level.
*/
float UTIL_WaterLevel( const Vector& vecPosition, float minz, float maxz );

/**
*	Creates bubbles that float upwards inside of a box.
*	@param mins Box mins.
*	@param maxs Box maxs.
*	@param count Number of bubbles to create.
*	@param flSpeed Speed of the bubbles.
*/
void UTIL_Bubbles( const Vector& mins, const Vector& maxs, int count, const float flSpeed = 8 );

/**
*	Creates a trail of bubbles that float upwards.
*	@param from Starting position.
*	@param to End position.
*	@param count Number of bubbles to create.
*	@param flSpeed Speed of the bubbles.
*/
void UTIL_BubbleTrail( const Vector& from, const Vector& to, int count, const float flSpeed = 8 );

/**
*	Converts a directional vector to angles.
*	@param vec Directional vector.
*	@return Angles.
*/
Vector UTIL_VecToAngles( const Vector& vec );

/**
*	Gets a velocity vector for the given amount of damage.
*/
Vector VecVelocityForDamage( const float flDamage );

// Testing strings for nullity
#define iStringNull 0
inline bool FStringNull( string_t iString ) { return iString == iStringNull; }

inline bool FStrEq( const char* sz1, const char* sz2 )
{
	return ( strcmp( sz1, sz2 ) == 0 );
}

void UTIL_MakeVectors( const Vector& vecAngles );

inline void UTIL_MakeVectorsPrivate( const Vector& vecAngles, Vector* p_vForward, Vector* p_vRight, Vector* p_vUp )
{
#ifdef CLIENT_DLL
	AngleVectors( vecAngles, p_vForward, p_vRight, p_vUp );
#else
	g_engfuncs.pfnAngleVectors( vecAngles, 
								reinterpret_cast<float*>( p_vForward ), 
								reinterpret_cast<float*>( p_vRight ), 
								reinterpret_cast<float*>( p_vUp ) );
#endif
}

/**
*	Like MakeVectors, but assumes pitch isn't inverted.
*/
inline void UTIL_MakeAimVectors( const Vector& vecAngles )
{
	Vector rgflVec = vecAngles;
	rgflVec[ 0 ] = -rgflVec[ 0 ];
	UTIL_MakeVectors( rgflVec );
}

void UTIL_MakeInvVectors( const Vector& vec, globalvars_t* pgv );

void UTIL_Sparks( const Vector& position );

void UTIL_Ricochet( const Vector& position, float scale );

#endif //GAME_SHARED_SHARED_GAME_UTILS_H