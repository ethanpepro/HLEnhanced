#ifndef GAME_SERVER_CSERVERGAMEINTERFACE_H
#define GAME_SERVER_CSERVERGAMEINTERFACE_H

#include "CBaseGameInterface.h"

/**
*	The server's representation of itself.
*/
class CServerGameInterface : public CBaseGameInterface
{
public:
	CServerGameInterface() = default;
	~CServerGameInterface() = default;

	/**
	*	Initializes the server.
	*	@return true on success, false on failure.
	*/
	bool Initialize();

	/**
	*	Shuts down the server.
	*	Should be called even if Initialize returned false.
	*/
	void Shutdown();

private:
	CServerGameInterface( const CServerGameInterface& ) = delete;
	CServerGameInterface& operator=( const CServerGameInterface& ) = delete;
};

extern CServerGameInterface g_Server;

#endif //GAME_SERVER_CSERVERGAMEINTERFACE_H