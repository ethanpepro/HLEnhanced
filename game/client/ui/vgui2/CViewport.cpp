#include "CViewport.h"

CViewport* g_pViewport = nullptr;

CViewport::CViewport()
{
}

void CViewport::Initialize( CreateInterfaceFn* pFactories, int iNumFactories )
{
}

void CViewport::Start()
{
}

vgui2::Panel* g_pPanel = nullptr;
vgui2::Panel* g_pPanel2 = nullptr;

void CViewport::SetParent( vgui2::VPANEL parent )
{
	g_pPanel = new vgui2::Panel();

	g_pPanel->SetParent( parent );

	g_pPanel->SetSize( 200, 200 );
	g_pPanel->SetPos( 10, 10 );

	g_pPanel->SetVisible( true );

	g_pPanel2 = new vgui2::Panel();

	g_pPanel2->SetParent( parent );

	g_pPanel2->SetSize( 100, 100 );
	g_pPanel2->SetPos( 10, 150 );

	g_pPanel2->SetVisible( true );
}

bool CViewport::UseVGUI1()
{
	return true;
}

void CViewport::HideScoreBoard()
{
}

void CViewport::HideAllVGUIMenu()
{
	//vgui2::surface()->UnlockCursor();
}

void CViewport::ActivateClientUI()
{
}

void CViewport::HideClientUI()
{
}

void CViewport::Shutdown()
{
}
