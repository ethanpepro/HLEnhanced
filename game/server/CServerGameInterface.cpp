#include "CServerGameInterface.h"

CServerGameInterface g_Server;

bool CServerGameInterface::Initialize()
{
	if( !InitFileSystem() )
		return false;

	return true;
}

void CServerGameInterface::Shutdown()
{
	ShutdownFileSystem();
}