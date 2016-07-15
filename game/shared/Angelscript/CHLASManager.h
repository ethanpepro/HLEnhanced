#ifndef GAME_SHARED_ANGELSCRIPT_CHLASMANAGER_H
#define GAME_SHARED_ANGELSCRIPT_CHLASMANAGER_H

#include <angelscript.h>

#include <Angelscript/CASManager.h>

class CASModule;
class CGameRules;

/**
*	Manages the Angelscript engine, the map script and handles some important map stage activities.
*/
class CHLASManager final
{
public:
	CHLASManager() = default;
	~CHLASManager() = default;

	void MessageCallback( asSMessageInfo* pMsg );

	/**
	*	Initializes Angelscript, registers the API and module types.
	*	@return true on success, false on failure.
	*/
	bool Initialize();

	/**
	*	Shuts down Angelscript.
	*/
	void Shutdown();

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
	CASManager m_Manager;

	CASModule* m_pModule = nullptr;

private:
	CHLASManager( const CHLASManager& ) = delete;
	CHLASManager& operator=( const CHLASManager& ) = delete;
};

extern CHLASManager g_ASManager;

#endif //GAME_SHARED_ANGELSCRIPT_CHLASMANAGER_H