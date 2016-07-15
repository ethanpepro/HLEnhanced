#include "CServerGameInterface.h"

CServerGameInterface g_Server;

bool CServerGameInterface::Initialize()
{
	if( !InitializeCommon() )
		return false;

	return true;
}

void CServerGameInterface::Shutdown()
{
	ShutdownCommon();
}