#include "CClientGameInterface.h"

CClientGameInterface g_Client;

bool CClientGameInterface::Initialize()
{
	if( !InitFileSystem() )
		return false;

	return true;
}

void CClientGameInterface::Shutdown()
{
	ShutdownFileSystem();
}