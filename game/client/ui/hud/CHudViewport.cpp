#include <vgui/IInputInternal.h>
#if 0
#include "vgui2/CClientMOTD.h"
#endif

#include "CHudViewport.h"

#include <vgui/ISurface.h>

void CHudViewport::Start()
{
	BaseClass::Start();
}

void CHudViewport::ActivateClientUI()
{
	BaseClass::ActivateClientUI();
}

void CHudViewport::CreateDefaultPanels()
{
#if 0
	AddNewPanel( CreatePanelByName( VIEWPORT_PANEL_MOTD ) );
#endif
}

IViewportPanel* CHudViewport::CreatePanelByName( const char* pszName )
{
	IViewportPanel* pPanel = nullptr;

#if 0
	if( Q_strcmp( VIEWPORT_PANEL_MOTD, pszName ) == 0 )
	{
		pPanel = new CClientMOTD( this );
	}
#endif

	return pPanel;
}
