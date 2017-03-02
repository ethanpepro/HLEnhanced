#include "hud.h"
#include "cl_util.h"

#include "CHudList.h"

#include "vgui_TeamFortressViewport.h"

#if USE_VGUI2
#include <vgui_controls/Panel.h>
#endif

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
	// In case we get messages before the first update -- time will be valid
	m_flTime = 1.0;
}

void CBaseHud::VidInit()
{
}

bool CBaseHud::Redraw( float flTime, bool intermission )
{
	m_flOldTime = m_flTime;	// save time of previous redraw
	m_flTime = flTime;
	m_flTimeDelta = ( double ) m_flTime - m_flOldTime;

	// Clock was reset, reset delta
	if( m_flTimeDelta < 0 )
		m_flTimeDelta = 0;

	// Bring up the scoreboard during intermission
	if( gViewPort )
	{
		if( m_bIntermission && !intermission )
		{
			// Have to do this here so the scoreboard goes away
			m_bIntermission = intermission;
			gViewPort->HideCommandMenu();
			gViewPort->HideScoreBoard();
			gViewPort->UpdateSpectatorPanel();
		}
		else if( !m_bIntermission && intermission )
		{
			m_bIntermission = intermission;
			gViewPort->HideCommandMenu();
			gViewPort->HideVGUIMenu();
			gViewPort->ShowScoreBoard();
			gViewPort->UpdateSpectatorPanel();

			// Take a screenshot if the client's got the cvar set
			if( CVAR_GET_FLOAT( "hud_takesshots" ) != 0 )
				m_flSnapshotTime = flTime + 1.0;	// Take a screenshot in a second
		}
	}

	if( m_flSnapshotTime && m_flSnapshotTime < flTime )
	{
		gEngfuncs.pfnClientCmd( "snapshot\n" );
		m_flSnapshotTime = 0;
	}

	m_bIntermission = intermission;

	return DoDraw( flTime, intermission );
}

void CBaseHud::DrawHudElements( float flTime, HudElementEvaluatorFn evaluatorFn, void* pUserData )
{
	ASSERT( evaluatorFn );

	auto count = HudList().GetElementCount();

	for( decltype( count ) index = 0; index < count; ++index )
	{
		auto pElem = HudList().GetElementByIndex( index );

#if USE_VGUI2
		// Visible?
		bool visible = pElem->ShouldDraw();

		pElem->SetActive( visible );

		// If it's a vgui panel, hide/show as appropriate
		vgui2::Panel *pPanel = dynamic_cast<vgui2::Panel*>( pElem );
		if( pPanel && pPanel->IsVisible() != visible )
		{
			pPanel->SetVisible( visible );
		}
		else if( !pPanel )
		{
			// All HUD elements should now derive from vgui!!!
			//TODO - Solokiller
			//Assert( false );
		}

		if( visible )
		{
			pElem->ProcessInput();
		}
#endif

		if( evaluatorFn( pElem, pUserData ) )
			pElem->Draw( flTime );
	}
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
