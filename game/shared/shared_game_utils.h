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

extern DLL_GLOBAL const Vector g_vecZero;

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

#define MAX_COM_TOKEN 1500

extern char com_token[ MAX_COM_TOKEN ];

//TODO: tidy the parse code, make it use user provided buffers - Solokiller
char *COM_Parse( char *data );

int COM_TokenWaiting( char *buffer );

char *memfgets( byte *pMemFile, int fileSize, int &filePos, char *pBuffer, int bufferSize );

#endif //GAME_SHARED_SHARED_GAME_UTILS_H