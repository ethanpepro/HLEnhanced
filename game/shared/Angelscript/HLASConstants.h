#ifndef GAME_SHARED_ANGELSCRIPT_HLASCONSTANTS_H
#define GAME_SHARED_ANGELSCRIPT_HLASCONSTANTS_H

namespace ModuleAccessMask
{
/**
*	Access masks for modules.
*/
enum ModuleAccessMask
{
	/**
	*	No access.
	*/
	NONE = 0,

	/**
	*	Shared API.
	*/
	SHARED = 1 << 0,

	/**
	*	Map script specific.
	*/
	MAPSCRIPT = SHARED | 1 << 1,

	/**
	*	Plugin script specific.
	*/
	PLUGIN = SHARED | 1 << 2,

	/**
	*	All scripts.
	*/
	ALL = SHARED | MAPSCRIPT | PLUGIN
};
}

const size_t AS_MAX_VARARGS = 16;

#endif //GAME_SHARED_ANGELSCRIPT_HLASCONSTANTS_H