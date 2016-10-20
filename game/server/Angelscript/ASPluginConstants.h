#ifndef GAME_SERVER_ANGELSCRIPT_ASPLUGINCONSTANTS_H
#define GAME_SERVER_ANGELSCRIPT_ASPLUGINCONSTANTS_H

#include <bitset>
#include <cstdint>

enum class PluginLifetime
{
	HOTRELOADABLE = 0,
	MAP,
	SERVER
};

/**
*	Parses the given string and returns the appropriate PluginLifetime constant for it.
*	Returns HOTRELOADABLE if the string is null or not a match to any other constant.
*/
PluginLifetime PluginLifetimeFromString( const char* const pszString );

/**
*	Returns the string representation of the given lifetime constant.
*/
const char* PluginLifetimeToString( const PluginLifetime lifetime );

namespace PluginFlag
{
/**
*	Plugin flags. Plugins use a bitset to store flags, so these aren't bit masks.
*/
enum PluginFlag
{
	NONE = 0,

	/**
	*	The plugin is flagged to be unloaded.
	*/
	UNLOAD,

	/**
	*	The plugin is flagged to be reloaded.
	*/
	LOAD,

	COUNT
};
}

using PluginFlags_t = std::bitset<PluginFlag::COUNT>;

#endif //GAME_SERVER_ANGELSCRIPT_ASPLUGINCONSTANTS_H
