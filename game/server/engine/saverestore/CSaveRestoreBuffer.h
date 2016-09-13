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
#ifndef GAME_SERVER_ENGINE_SAVERESTORE_CSAVERESTOREBUFFER_H
#define GAME_SERVER_ENGINE_SAVERESTORE_CSAVERESTOREBUFFER_H

//TODO: all of these could probably go into a single header that forward declares everything - Solokiller
typedef struct saverestore_s SAVERESTOREDATA;
typedef struct entvars_s entvars_t;
typedef struct edict_s edict_t;
typedef int EOFFSET;
class CBaseEntity;

namespace engine
{
enum FIELDTYPE
{
	FIELD_FLOAT = 0,		// Any floating point value
	FIELD_STRING,			// A string ID (return from ALLOC_STRING)
	FIELD_ENTITY,			// An entity offset (EOFFSET)
	FIELD_CLASSPTR,			// CBaseEntity *
	FIELD_EHANDLE,			// Entity handle
	FIELD_EVARS,			// EVARS *
	FIELD_EDICT,			// edict_t *, or edict_t *  (same thing)
	FIELD_VECTOR,			// Any vector
	FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
	FIELD_POINTER,			// Arbitrary data pointer... to be removed, use an array of FIELD_CHARACTER
	FIELD_INTEGER,			// Any integer or enum
	FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)
	FIELD_BOOLEAN,			// boolean, I may use this as a hint for compression
	FIELD_SHORT,			// 2 byte integer
	FIELD_CHARACTER,		// a byte
	FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
	FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
	FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)

	FIELD_TYPECOUNT,		// MUST BE LAST
};

struct TYPEDESCRIPTION
{
	FIELDTYPE		fieldType;
	const char*		fieldName;
	int				fieldOffset;
	short			fieldSize;
	short			flags;
};

extern const int g_SaveRestoreSizes[];

extern const TYPEDESCRIPTION gEntvarsDescription[];

extern const size_t gEntvarsCount;

/**
*	A buffer that contains save/restore data.
*	Used for engine save/restore operations.
*/
class CSaveRestoreBuffer
{
public:
	CSaveRestoreBuffer( void );
	CSaveRestoreBuffer( SAVERESTOREDATA *pdata );
	~CSaveRestoreBuffer( void );

	int			EntityIndex( entvars_t *pevLookup );
	int			EntityIndex( edict_t *pentLookup );
	int			EntityIndex( EOFFSET eoLookup );
	int			EntityIndex( CBaseEntity *pEntity );

	int			EntityFlags( int entityIndex, int flags ) { return EntityFlagsSet( entityIndex, 0 ); }
	int			EntityFlagsSet( int entityIndex, int flags );

	edict_t		*EntityFromIndex( int entityIndex );

	unsigned short	TokenHash( const char *pszToken );

protected:
	SAVERESTOREDATA		*m_pdata;
	void		BufferRewind( int size );
	unsigned int	HashString( const char *pszToken );
};
}

#endif //GAME_SERVER_ENGINE_SAVERESTORE_CSAVERESTOREBUFFER_H
