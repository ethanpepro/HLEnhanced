#include <clocale>

//TODO: remove once VGUI1 is removed.
#ifdef USE_VGUI2
#undef VGUI_H

#include "../../source_sdk/public/vgui/VGUI.h"
#include "../../source_sdk/public/vgui/IVGui.h"
#include "../../source_sdk/public/vgui/ischeme.h"
#include "../../source_sdk/public/vgui_controls/Controls.h"
#include "../../source_sdk/public/vgui_controls/Panel.h"
#include "../../source_sdk/public/vgui_controls/Frame.h"

#include <vgui/isurface.h>
#endif

#include "IGameUIFuncs.h"

#include "CClientVGUI.h"

namespace
{
CClientVGUI g_ClientVGUI;

IGameUIFuncs* g_GameUIFuncs = nullptr;
}

IClientVGUI* clientVGUI()
{
	return &g_ClientVGUI;
}

IGameUIFuncs* gameUIFuncs()
{
	return g_GameUIFuncs;
}

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CClientVGUI, IClientVGUI, ICLIENTVGUI_NAME, g_ClientVGUI );

CClientVGUI::CClientVGUI()
{
}

void CClientVGUI::Initialize( CreateInterfaceFn* pFactories, int iNumFactories )
{
	/*
	*	Factories in the given array:
	*	engine
	*	vgui2
	*	filesystem
	*	chrome HTML
	*	GameUI
	*	client (this library)
	*/

	//4 factories to use.
	assert( static_cast<size_t>( iNumFactories ) >= NUM_FACTORIES - 1 );

	m_FactoryList[ 0 ] = Sys_GetFactoryThis();

	for( size_t uiIndex = 0; uiIndex < NUM_FACTORIES - 1; ++uiIndex )
	{
		m_FactoryList[ uiIndex + 1 ] = pFactories[ uiIndex ];
	}

	//vgui2::VGui_InitInterfacesList( "CLIENT", pFactories, NUM_FACTORIES );
	//
	//vgui2::scheme()->LoadSchemeFromFile( "Resource/ClientScheme.res", "ClientScheme" );
	//vgui2::scheme()->LoadSchemeFromFile( "Resource/TutorScheme.res", "TutorScheme" );
	g_GameUIFuncs = ( IGameUIFuncs* ) pFactories[ 0 ]( IGAMEUIFUNCS_NAME, nullptr );
}

void CClientVGUI::Start()
{
}

vgui2::Panel* g_pPanel = nullptr;
vgui2::Panel* g_pPanel2 = nullptr;

void CClientVGUI::SetParent( vgui2::VPANEL parent )
{
	//g_pPanel = new vgui2::Panel();
	//
	//g_pPanel->SetParent( parent );
	//
	//g_pPanel->SetSize( 200, 200 );
	//g_pPanel->SetPos( 10, 10 );
	//
	//g_pPanel->SetVisible( true );
	//
	//g_pPanel2 = new vgui2::Panel();
	//
	//g_pPanel2->SetParent( parent );
	//
	//g_pPanel2->SetSize( 100, 100 );
	//g_pPanel2->SetPos( 10, 150 );
	//
	//g_pPanel2->SetVisible( true );
}

int CClientVGUI::UseVGUI1()
{
	return true;
}

void CClientVGUI::HideScoreBoard()
{
}

void CClientVGUI::HideAllVGUIMenu()
{
	//vgui2::surface()->UnlockCursor();
}

void CClientVGUI::ActivateClientUI()
{
}

void CClientVGUI::HideClientUI()
{
}

void CClientVGUI::Shutdown()
{
}