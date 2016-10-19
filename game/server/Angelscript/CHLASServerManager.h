#ifndef GAME_SERVER_ANGELSCRIPT_CHLASSERVERMANAGER_H
#define GAME_SERVER_ANGELSCRIPT_CHLASSERVERMANAGER_H

#include "Angelscript/CHLASManager.h"

#include "CASPluginManager.h"

class CASModule;
class CGameRules;

/**
*	Manages the map script and handles some important map stage activities.
*/
class CHLASServerManager final : public CHLASManager
{
public:
	CHLASServerManager();

	CASManager& GetASManager() { return m_Manager; }

	CASPluginManager& GetPluginManager() { return m_PluginManager; }

	bool Initialize() override;

	void Shutdown() override;

	void Think();

	/**
	*	Creates the gamerules object if the script has implemented it.
	*	@return Gamerules instance, or null if the script didn't provide one.
	*/
	CGameRules* CreateGameRules();

	/**
	*	A new map has been started. Handle map script compilation, if any script name was provided.
	*	Calls MapInit.
	*	@param pszMapScriptFileName Name of the map script to load.
	*/
	void WorldCreated( const char* const pszMapScriptFileName );

	/**
	*	All entities have been created and activated.
	*	Calls MapActivate.
	*/
	void WorldActivated();

	/**
	*	The map has ended, and worldspawn is being removed.
	*	Calls MapEnd.
	*/
	void WorldEnded();

private:
	CASPluginManager m_PluginManager;

	CASModule* m_pModule = nullptr;
};

extern CHLASServerManager g_ASManager;

#endif //GAME_SERVER_ANGELSCRIPT_CHLASSERVERMANAGER_H
