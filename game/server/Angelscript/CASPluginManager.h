#ifndef GAME_SERVER_ANGELSCRIPT_CASPLUGINMANAGER_H
#define GAME_SERVER_ANGELSCRIPT_CASPLUGINMANAGER_H

#include <vector>

class CASModule;
class CHLASServerManager;

class CASPluginManager final
{
private:
	using PluginList_t = std::vector<CASModule*>;

public:
	CASPluginManager( CHLASServerManager& ASManager );
	~CASPluginManager() = default;

	/**
	*	A new map has started, handle pending plugin actions.
	*/
	void WorldCreated();

	void Think();

private:
	void LoadPluginsFile( const char* const pszPluginFile );

	bool LoadPlugin( const char* const pszName, const char* const pszScript );

private:
	CHLASServerManager& m_ASManager;

	bool m_bLoadedPluginFile = false;

	PluginList_t m_PluginList;

	float m_flPrevThinkTime = 1;

private:
	CASPluginManager( const CASPluginManager& ) = delete;
	CASPluginManager& operator=( const CASPluginManager& ) = delete;
};

#endif //GAME_SERVER_ANGELSCRIPT_CASPLUGINMANAGER_H
