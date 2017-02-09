//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================
#ifndef GAME_CLIENT_UI_VGUI_VGUI_INPUTSIGNALHANDLERS_H
#define GAME_CLIENT_UI_VGUI_VGUI_INPUTSIGNALHANDLERS_H

#include <VGUI_Button.h>
#include <VGUI_InputSignal.h>

#include "vgui_CommandButton.h"

class CMenuPanel;
class DragNDropPanel;

/**
*	@file
*
*	Generic input signal handlers
*/

//TODO: Use CDefaultInputSignal to avoid having to manually provide empty handlers. - Solokiller

class CHandler_MenuButtonOver : public vgui::InputSignal
{
private:
	int			m_iButton;
	CMenuPanel	*m_pMenuPanel;
public:
	CHandler_MenuButtonOver( CMenuPanel *pPanel, int iButton )
	{
		m_iButton = iButton;
		m_pMenuPanel = pPanel;
	}

	void cursorEntered( vgui::Panel *panel );

	void cursorMoved( int x, int y, vgui::Panel* panel ) {};
	void mousePressed( vgui::MouseCode code, vgui::Panel* panel ) {};
	void mouseReleased( vgui::MouseCode code, vgui::Panel* panel ) {};
	void mouseDoublePressed( vgui::MouseCode code, vgui::Panel* panel ) {};
	void cursorExited( vgui::Panel* panel ) {};
	void mouseWheeled( int delta, vgui::Panel* panel ) {};
	void keyPressed( vgui::KeyCode code, vgui::Panel* panel ) {};
	void keyTyped( vgui::KeyCode code, vgui::Panel* panel ) {};
	void keyReleased( vgui::KeyCode code, vgui::Panel* panel ) {};
	void keyFocusTicked( vgui::Panel* panel ) {};
};

class CHandler_ButtonHighlight : public vgui::InputSignal
{
private:
	vgui::Button *m_pButton;
public:
	CHandler_ButtonHighlight( vgui::Button *pButton )
	{
		m_pButton = pButton;
	}

	virtual void cursorEntered( vgui::Panel* panel )
	{
		m_pButton->setArmed( true );
	};
	virtual void cursorExited( vgui::Panel* Panel )
	{
		m_pButton->setArmed( false );
	};
	virtual void mousePressed( vgui::MouseCode code, vgui::Panel* panel ) {};
	virtual void mouseReleased( vgui::MouseCode code, vgui::Panel* panel ) {};
	virtual void cursorMoved( int x, int y, vgui::Panel* panel ) {};
	virtual void mouseDoublePressed( vgui::MouseCode code, vgui::Panel* panel ) {};
	virtual void mouseWheeled( int delta, vgui::Panel* panel ) {};
	virtual void keyPressed( vgui::KeyCode code, vgui::Panel* panel ) {};
	virtual void keyTyped( vgui::KeyCode code, vgui::Panel* panel ) {};
	virtual void keyReleased( vgui::KeyCode code, vgui::Panel* panel ) {};
	virtual void keyFocusTicked( vgui::Panel* panel ) {};
};

//-----------------------------------------------------------------------------
// Purpose: Special handler for highlighting of command menu buttons
//-----------------------------------------------------------------------------
class CHandler_CommandButtonHighlight : public CHandler_ButtonHighlight
{
private:
	CommandButton *m_pCommandButton;
public:
	CHandler_CommandButtonHighlight( CommandButton *pButton )
		: CHandler_ButtonHighlight( pButton )
	{
		m_pCommandButton = pButton;
	}

	virtual void cursorEntered( vgui::Panel *panel )
	{
		m_pCommandButton->cursorEntered();
	}

	virtual void cursorExited( vgui::Panel *panel )
	{
		m_pCommandButton->cursorExited();
	}
};

#endif //GAME_CLIENT_UI_VGUI_VGUI_INPUTSIGNALHANDLERS_H
