#include "CClientGameInterface.h"

CClientGameInterface g_Client;

bool CClientGameInterface::Initialize()
{
	if( !InitializeCommon() )
		return false;

	return true;
}

void CClientGameInterface::Shutdown()
{
	ShutdownCommon();
}