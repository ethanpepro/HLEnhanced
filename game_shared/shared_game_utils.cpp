#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "shared_game_utils.h"

DLL_GLOBAL const Vector	g_vecZero = Vector( 0, 0, 0 );

class CBaseEntity;

CBaseEntity* GET_PRIVATE( edict_t* pent )
{
	if( pent )
		return reinterpret_cast<CBaseEntity*>( pent->pvPrivateData );

	return nullptr;
}

CHashStringPool g_StringPool;

bool FNullEnt( const CBaseEntity* pEntity )
{
	return pEntity == nullptr || FNullEnt( pEntity->pev );
}