#ifndef GAME_CLIENT_UI_VGUI2_CBASEVIEWPORT_H
#define GAME_CLIENT_UI_VGUI2_CBASEVIEWPORT_H

#include "IClientVGUI.h"

#include "utlvector.h"

#include "vgui_controls/editablepanel.h"

class CBackGroundPanel;
class IViewportPanel;

/**
*	Base class for VGUI2 viewports.
*/
class CBaseViewport : public IClientVGUI, public vgui2::EditablePanel
{
public:
	typedef CBaseViewport ThisClass;
	typedef vgui2::EditablePanel BaseClass;

public:
	CBaseViewport();
	virtual ~CBaseViewport();

	//IClientVGUI methods. Passed through to us by CClientVGUI.
	void Initialize( CreateInterfaceFn* pFactories, int iNumFactories ) override;

	void Start() override;

	void SetParent( vgui2::VPANEL parent ) override;

	int UseVGUI1() override;

	void HideScoreBoard() override;

	void HideAllVGUIMenu() override;

	void ActivateClientUI() override;

	void HideClientUI() override;

	void Shutdown() override;

	//Panel overrides
	void OnThink() override;

	//CBaseViewport
	virtual void Layout();

	//Panels
	virtual void CreateDefaultPanels();

	virtual void UpdateAllPanels();

	/**
	*	Creates a viewport panel by name.
	*/
	virtual IViewportPanel* CreatePanelByName( const char* pszName );

	/**
	*	Finds a panel by name.
	*/
	virtual IViewportPanel* FindPanelByName( const char* pszName );

	/**
	*	Adds a viewport panel to the viewport.
	*	@param pPanel Panel to add. Must be non-null and not already added.
	*	@return Whether the panel was added to the viewport.
	*/
	virtual bool AddNewPanel( IViewportPanel* pPanel );

	/**
	*	Shows/hides the panel with the given name.
	*	@param pszName Name of the panel.
	*	@param bState State to set. If false, the last active panel is restored if it exists.
	*/
	virtual void ShowPanel( const char* pszName, const bool bState );

	/**
	*	Shows/hides the given panel.
	*	@param pPanel Panel to show or hide.
	*	@param bState State to set. If false, the last active panel is restored if it exists.
	*/
	virtual void ShowPanel( IViewportPanel* pPanel, const bool bState );

	virtual void RemoveAllPanels();

	virtual IViewportPanel* GetActivePanel();

	virtual IViewportPanel* GetLastActivePanel();

	//BackGround
	virtual bool IsBackGroundVisible() const;

	virtual void ShowBackGround( const bool bState );

private:
	int m_OldSize[ 2 ] = { -1, -1 };
	int m_nRootSize[ 2 ] = { -1, -1 };

	CBackGroundPanel* m_pBackGround = nullptr;

	CUtlVector<IViewportPanel*> m_Panels;

	IViewportPanel* m_pActivePanel = nullptr;
	IViewportPanel* m_pLastActivePanel = nullptr;
};

extern CBaseViewport* g_pViewport;

#endif //GAME_CLIENT_UI_VGUI2_CBASEVIEWPORT_H
