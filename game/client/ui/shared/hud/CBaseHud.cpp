#include "hud.h"
#include "cl_util.h"

#include "CHudList.h"

#include "CBaseHud.h"

CBaseHud::CBaseHud()
{
}

CBaseHud::~CBaseHud()
{
}

void CBaseHud::InitHudElements()
{
	HudList().InitHudElements( *this );
}

void CBaseHud::GameShutdown()
{
	HudList().GameShutdown();
}
