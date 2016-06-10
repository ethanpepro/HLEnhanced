#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "shared_game_utils.h"

class CBaseEntity;

CBaseEntity* GET_PRIVATE( edict_t* pent )
{
	if( pent )
		return reinterpret_cast<CBaseEntity*>( pent->pvPrivateData );

	return nullptr;
}