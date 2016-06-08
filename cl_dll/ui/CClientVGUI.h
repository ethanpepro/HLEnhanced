#ifndef CL_UI_CCLIENTVGUI_H
#define CL_UI_CCLIENTVGUI_H

#include "IClientVGUI.h"

class CClientVGUI : public IClientVGUI
{
public:
	/**
	*	Constructor.
	*/
	CClientVGUI();

	void Initialize( CreateInterfaceFn* pFactories, int iNumFactories ) override final;

	void Start() override final;

	void SetParent( unsigned int pParentPanel ) override final;

	int UseVGUI1() override final;

	void HideScoreBoard() override final;

	void HideAllVGUIMenu() override final;

	void ActivateClientUI() override final;

	void HideClientUI() override final;

	void Shutdown() override final;

private:
	CClientVGUI( const CClientVGUI& ) = delete;
	CClientVGUI& operator=( const CClientVGUI& ) = delete;
};

#endif //CL_UI_CCLIENTVGUI_H