#ifndef GAME_SHARED_ANGELSCRIPT_CHLASMANAGER_H
#define GAME_SHARED_ANGELSCRIPT_CHLASMANAGER_H

#include <angelscript.h>

#include <Angelscript/CASManager.h>

class CASModule;
class CGameRules;

/**
*	Manages the Angelscript engine, the map script and handles some important map stage activities.
*/
class CHLASManager
{
public:
	CHLASManager() = default;
	~CHLASManager() = default;

	void MessageCallback( asSMessageInfo* pMsg );

	/**
	*	Initializes Angelscript, registers the API and module types.
	*	@return true on success, false on failure.
	*/
	virtual bool Initialize() = 0;

	/**
	*	Shuts down Angelscript.
	*/
	virtual void Shutdown();

protected:
	CASManager m_Manager;

private:
	CHLASManager( const CHLASManager& ) = delete;
	CHLASManager& operator=( const CHLASManager& ) = delete;
};

#endif //GAME_SHARED_ANGELSCRIPT_CHLASMANAGER_H