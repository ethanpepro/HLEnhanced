#ifndef GAME_SHARED_SHARED_GAME_UTILS_H
#define GAME_SHARED_SHARED_GAME_UTILS_H

/**
*	dll agnostic game utility functionality - Solokiller
*/

class CBaseEntity;

/**
*	Gets the CBaseEntity instance out of the edict, if it's a valid instance.
*	This used to return void*. This led to unsafe casts that could cause invalid conversions (and has, in some cases). Use static_cast or dynamic_cast. - Solokiller
*/
CBaseEntity* GET_PRIVATE( edict_t* pent );

#endif //GAME_SHARED_SHARED_GAME_UTILS_H