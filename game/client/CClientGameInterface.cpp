#include "hl/hl_weapons.h"

#include "CClientGameInterface.h"

CClientGameInterface g_Client;

bool CClientGameInterface::Initialize()
{
	if( !InitializeCommon() )
		return false;

	//Init the HUD weapons code for prediction.
	HUD_InitClientWeapons();

	return true;
}

void CClientGameInterface::Shutdown()
{
	ShutdownCommon();
}