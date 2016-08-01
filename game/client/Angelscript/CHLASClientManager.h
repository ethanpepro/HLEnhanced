#ifndef GAME_SERVER_ANGELSCRIPT_CHLASCLIENTMANAGER_H
#define GAME_SERVER_ANGELSCRIPT_CHLASCLIENTMANAGER_H

#include "Angelscript/CHLASManager.h"

class CASModule;

/**
*	Manages the map script and handles some important map stage activities.
*/
class CHLASClientManager final : public CHLASManager
{
public:

	bool Initialize() override;

	void Shutdown() override;

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
	CASModule* m_pModule = nullptr;
};

extern CHLASClientManager g_ASManager;

#endif //GAME_SERVER_ANGELSCRIPT_CHLASCLIENTMANAGER_H
