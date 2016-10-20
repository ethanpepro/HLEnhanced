#ifndef GAME_SERVER_ANGELSCRIPT_CASPLUGINMANAGER_H
#define GAME_SERVER_ANGELSCRIPT_CASPLUGINMANAGER_H

#include <string>
#include <vector>

#include "ASPluginConstants.h"

class CASModule;
class CHLASServerManager;

/**
*	Manages Angelscript plugins.
*/
class CASPluginManager final
{
private:
	using PluginList_t = std::vector<CASModule*>;
	using LoadList_t = std::vector<std::string>;

	/**
	*	The amount of time that can pass between plugin flag checks.
	*/
	static const float PLUGIN_FLAG_INTERVAL;

public:
	CASPluginManager( CHLASServerManager& ASManager );
	~CASPluginManager() = default;

	/**
	*	@return The name of the plugin list file to load.
	*/
	const char* GetPluginListFile() const;

	bool Initialize();

	void Shutdown();

	/**
	*	A new map has started, handle pending plugin actions.
	*/
	void WorldCreated();

	/**
	*	Should be called every frame. Periodically checks for plugins to unload and reload, updates the scheduler for each plugin.
	*/
	void Think();

	size_t GetPluginCount() const { return m_PluginList.size(); }

	CASModule* GetPluginByIndex( const size_t uiIndex );

	/**
	*	Finds a plugin by name (case insensitive).
	*/
	CASModule* FindPluginByName( const char* const pszName );

	/**
	*	Queue up a plugin to be delay loaded when the next map starts.
	*/
	void AddDelayLoadPlugin( const char* const pszName );

	/**
	*	If set, the next time a map starts all plugins that were newly added to the plugin list file will be loaded.
	*/
	void MarkLoadNewPlugins();

	/**
	*	Sets the next time that the plugin flag check should occur. Controls when hot reloadable plugins are unloaded and reloaded.
	*/
	void SetNextPluginFlagCheckTime( const float flTime )
	{
		m_flNextPluginFlagCheck = flTime;
	}

private:
	/**
	*	Loads the given plugin list file.
	*	@param pszPluginFile Name of the file to load.
	*	@param[ in, out ] pPluginsToLoad Optional.
	*						If not null, this is a list of plugins to load from the file.
	*						This list is modified to contain only plugin names that were not found in the file.
	*/
	void LoadPluginsFile( const char* const pszPluginFile, LoadList_t* pPluginsToLoad );

	/**
	*	Loads a single plugin. Checks for duplicate names before loading the plugin.
	*	@param pszName Name of the plugin.
	*	@param pszScript Script to load.
	*	@param lifetime Minimum lifetime for this plugin.
	*	@return Whether the plugin was successfully loaded.
	*/
	bool LoadPlugin( const char* const pszName, const char* const pszScript, const PluginLifetime lifetime );

	/**
	*	Unloads the given plugin. The module is no longer usable after this call.
	*/
	void UnloadPlugin( CASModule* pPlugin );

	/**
	*	Processes the plugin flags for all loaded plugins.
	*	If they are marked to unload or reload and their lifetime is smaller than or equal to the given lifetime,
	*	the necessary actions are performed.
	*	@param lifetime Lifetime to check against.
	*	@param[ out ] List of plugins that need to be loaded after having been unloaded.
	*/
	void ProcessPluginLoadFlags( const PluginLifetime lifetime, LoadList_t& pluginsToLoad );

private:
	CHLASServerManager& m_ASManager;

	bool m_bShouldLoadPluginFile = true;

	PluginList_t m_PluginList;

	float m_flPrevThinkTime = 1;

	LoadList_t m_DelayLoadList;

	float m_flNextPluginFlagCheck = 1;

private:
	CASPluginManager( const CASPluginManager& ) = delete;
	CASPluginManager& operator=( const CASPluginManager& ) = delete;
};

#endif //GAME_SERVER_ANGELSCRIPT_CASPLUGINMANAGER_H
