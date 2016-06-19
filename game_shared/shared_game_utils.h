#ifndef GAME_SHARED_SHARED_GAME_UTILS_H
#define GAME_SHARED_SHARED_GAME_UTILS_H

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

extern DLL_GLOBAL const Vector g_vecZero;

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

char* UTIL_VarArgs( const char* pszFormat, ... );

// for handy use with ClientPrint params
//TODO: replace with multiple buffer implementation - Solokiller
char* UTIL_dtos1( int d );
char* UTIL_dtos2( int d );
char* UTIL_dtos3( int d );
char* UTIL_dtos4( int d );

void UTIL_StripToken( const char *pKey, char *pDest );// for redundant keynames

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

#endif //GAME_SHARED_SHARED_GAME_UTILS_H