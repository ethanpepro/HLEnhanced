#ifndef GAME_CLIENT_UI_VGUI2_CVIEWPORT_H
#define GAME_CLIENT_UI_VGUI2_CVIEWPORT_H

#include "../../source_sdk/public/vgui_controls/panel.h"

class CBackgroundPanel;

/**
*	Represents the VGUI2 viewport.
*/
class CViewport
{
public:
	CViewport();

	void Initialize( CreateInterfaceFn* pFactories, int iNumFactories );

	void Start();

	void SetParent( vgui2::VPANEL parent );

	bool UseVGUI1();

	void HideScoreBoard();

	void HideAllVGUIMenu();

	void ActivateClientUI();

	void HideClientUI();

	void Shutdown();

private:
	CBackgroundPanel* m_pBackground = nullptr;
};

extern CViewport* g_pViewport;

#endif //GAME_CLIENT_UI_VGUI2_CVIEWPORT_H
