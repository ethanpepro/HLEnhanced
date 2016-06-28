#ifndef GAME_CLIENT_CCLIENTGAMEINTERFACE_H
#define GAME_CLIENT_CCLIENTGAMEINTERFACE_H

#include "CBaseGameInterface.h"

/**
*	The client's representation of itself.
*/
class CClientGameInterface : public CBaseGameInterface
{
public:
	CClientGameInterface() = default;
	~CClientGameInterface() = default;

	/**
	*	Initializes the client.
	*	@return true on success, false on failure.
	*/
	bool Initialize();

	/**
	*	Shuts down the client.
	*	Should be called even if Initialize returned false.
	*/
	void Shutdown();

private:
	CClientGameInterface( const CClientGameInterface& ) = delete;
	CClientGameInterface& operator=( const CClientGameInterface& ) = delete;
};

extern CClientGameInterface g_Client;

#endif //GAME_CLIENT_CCLIENTGAMEINTERFACE_H