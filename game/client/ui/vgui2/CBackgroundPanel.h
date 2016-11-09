#ifndef GAME_CLIENT_UI_VGUI2_CBACKGROUNDPANEL_H
#define GAME_CLIENT_UI_VGUI2_CBACKGROUNDPANEL_H

#include "../../source_sdk/public/vgui_controls/frame.h"

class CBackgroundPanel : public vgui2::Frame
{
public:
	DECLARE_CLASS_SIMPLE( CBackgroundPanel, vgui2::Frame );

public:
	CBackgroundPanel( vgui2::Panel* pParent );
	
	void OnMousePressed( vgui2::MouseCode code ) override {}

	vgui2::VPANEL IsWithinTraverse( int x, int y, bool traversePopups ) override { return 0; }

	void ApplySchemeSettings( vgui2::IScheme* pScheme ) override;

	void PerformLayout() override;
};

#endif //GAME_CLIENT_UI_VGUI2_CBACKGROUNDPANEL_H
