#include "hud.h"
#include "cl_util.h"

#include "CHudList.h"

#include "CBaseHud.h"

namespace
{
static CBaseHud* g_pHud = nullptr;
}

CBaseHud& Hud()
{
	ASSERT( g_pHud );

	return *g_pHud;
}

void SetHud( CBaseHud* pHud, bool bDeleteOldHud )
{
	if( bDeleteOldHud && g_pHud )
		delete g_pHud;

	g_pHud = pHud;
}

CBaseHud::CBaseHud()
{
}

CBaseHud::~CBaseHud()
{
}

void CBaseHud::Init()
{
}

void CBaseHud::VidInit()
{
}

bool CBaseHud::Redraw( float flTime, bool intermission )
{
	return true;
}

bool CBaseHud::UpdateClientData( client_data_t* cdata )
{
	auto bChanged = PreThinkUpdateClient( cdata );

	Think();

	bChanged = PostThinkUpdateClient( cdata ) || bChanged;

	return bChanged;
}

bool CBaseHud::PreThinkUpdateClient( client_data_t* cdata )
{
	return false;
}

bool CBaseHud::PostThinkUpdateClient( client_data_t* cdata )
{
	return false;
}

void CBaseHud::Think()
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
