#include "hud.h"
#include "cl_dll.h"

#include "com_weapons.h"

#include "CClientGameInterface.h"

CClientGameInterface g_Client;

bool CClientGameInterface::Initialize()
{
	if( !InitializeCommon() )
		return false;

	//Init ASAP so functions like AlertMessage get set up.
	CL_SetupServerSupport();

	return true;
}

void CClientGameInterface::Shutdown()
{
	ShutdownCommon();
}