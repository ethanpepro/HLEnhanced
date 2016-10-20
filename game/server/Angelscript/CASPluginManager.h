#ifndef GAME_SERVER_ANGELSCRIPT_CASPLUGINMANAGER_H
#define GAME_SERVER_ANGELSCRIPT_CASPLUGINMANAGER_H

#include <string>
#include <vector>

#include "ASPluginConstants.h"

class CASModule;
class CHLASServerManager;

class CASPluginManager final
{
private:
	using PluginList_t = std::vector<CASModule*>;
	using LoadList_t = std::vector<std::string>;

	static const float PLUGIN_FLAG_INTERVAL;

public:
	CASPluginManager( CHLASServerManager& ASManager );
	~CASPluginManager() = default;

	const char* GetPluginListFile() const;

	bool Initialize();

	void Shutdown();

	/**
	*	A new map has started, handle pending plugin actions.
	*/
	void WorldCreated();

	void Think();

	size_t GetPluginCount() const { return m_PluginList.size(); }

	CASModule* GetPluginByIndex( const size_t uiIndex );

	CASModule* FindPluginByName( const char* const pszName );

	/**
	*	Queue up a plugin to be delay loaded.
	*/
	void AddDelayLoadPlugin( const char* const pszName );

	void MarkLoadNewPlugins();

	void SetNextPluginFlagCheckTime( const float flTime )
	{
		m_flNextPluginFlagCheck = flTime;
	}

private:
	void LoadPluginsFile( const char* const pszPluginFile, LoadList_t* pPluginsToLoad );

	bool LoadPlugin( const char* const pszName, const char* const pszScript, const PluginLifetime lifetime );

	void UnloadPlugin( CASModule* pPlugin );

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
