#include "../../source_sdk/public/vgui/isurface.h"

#include "CBackgroundPanel.h"

CBackgroundPanel::CBackgroundPanel( vgui2::Panel* pParent )
	: BaseClass( pParent, "ViewPortBackGround" )
{
}

void CBackgroundPanel::ApplySchemeSettings( vgui2::IScheme* pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	const auto color = pScheme->GetColor( "ViewportBG", SDK_Color( 0, 0, 0, 0 ) );

	SetBgColor( color );
	SetInFocusBgColor( color );
	SetOutOfFocusBgColor( color );
}

void CBackgroundPanel::PerformLayout()
{
	//Resize ourselves to the screen's size to fill the entire viewport.
	int w, h;

	vgui2::surface()->GetScreenSize( w, h );

	SetBounds( 0, 0, w, h );

	BaseClass::PerformLayout();
}
