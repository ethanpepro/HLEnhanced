#ifndef GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
#define GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H

#include "shared_game_utils.h"
//TODO: need to move DataMapping out of entities. - Solokiller
#include "entities/DataMapping.h"

#include "CHudList.h"

/**
*	Base class for the Hud.
*/
class CBaseHud
{
public:
	DECLARE_CLASS_NOBASE( CBaseHud );

public:
	CBaseHud();
	virtual ~CBaseHud();

	virtual void InitHudElements();

	/**
	*	Called when the game is being shut down.
	*/
	virtual void GameShutdown();

private:
	CBaseHud( const CBaseHud& ) = delete;
	CBaseHud& operator=( const CBaseHud& ) = delete;
};

#endif //GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
