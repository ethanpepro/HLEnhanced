#include "hud.h"
#include "cl_util.h"

#include "CClientVGUI.h"

EXPOSE_SINGLE_INTERFACE( CClientVGUI, IClientVGUI, ICLIENTVGUI_NAME );

CClientVGUI::CClientVGUI()
{
}

void CClientVGUI::Initialize( CreateInterfaceFn* pFactories, int iNumFactories )
{
	gEngfuncs.Con_Printf( "Initialize\n" );
}

void CClientVGUI::Start()
{
	gEngfuncs.Con_Printf( "Start\n" );
}

void CClientVGUI::SetParent( unsigned int pParentPanel )
{
	gEngfuncs.Con_Printf( "SetParent\n" );
}

int CClientVGUI::UseVGUI1()
{
	return true;
}

void CClientVGUI::HideScoreBoard()
{
	gEngfuncs.Con_Printf( "HideScoreBoard\n" );
}

void CClientVGUI::HideAllVGUIMenu()
{
	gEngfuncs.Con_Printf( "HideAllVGUIMenu\n" );
}

void CClientVGUI::ActivateClientUI()
{
	gEngfuncs.Con_Printf( "ActivateClientUI\n" );
}

void CClientVGUI::HideClientUI()
{
	gEngfuncs.Con_Printf( "HideClientUI\n" );
}

void CClientVGUI::Shutdown()
{
	gEngfuncs.Con_Printf( "Shutdown\n" );
}