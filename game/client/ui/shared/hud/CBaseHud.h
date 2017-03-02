#ifndef GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
#define GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H

#include "shared_game_utils.h"
//TODO: need to move DataMapping out of entities. - Solokiller
#include "entities/DataMapping.h"

#include "CHudList.h"

class CHudElementRegistry;

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

	/**
	*	@return The Hud element registry for this Hud.
	*/
	virtual CHudElementRegistry& GetHudElementRegistry() = 0;

	virtual void Init();

	virtual void VidInit();

	virtual bool Redraw( float flTime, bool intermission );

	bool UpdateClientData( client_data_t* cdata );

protected:
	/**
	*	Called before the Hud runs think.
	*/
	virtual bool PreThinkUpdateClient( client_data_t* cdata );

	/**
	*	Called after the Hud has ran think.
	*/
	virtual bool PostThinkUpdateClient( client_data_t* cdata );

	/**
	*	Called every time the client's data has changed, after PreThinkUpdateClient and before PostThinkUpdateClient.
	*/
	virtual void Think();

public:
	virtual void InitHudElements();

	/**
	*	Called when the game is being shut down.
	*/
	virtual void GameShutdown();

private:
	CBaseHud( const CBaseHud& ) = delete;
	CBaseHud& operator=( const CBaseHud& ) = delete;
};

/**
*	@return The current Hud. If no Hud was set, this will cause a crash.
*/
CBaseHud& Hud();

/**
*	Sets the current Hud.
*	@param pHud Hud to set. Can be null
*	@param bDeleteOldHud If true, the old Hud is deleted
*/
void SetHud( CBaseHud* pHud, bool bDeleteOldHud = true );

#endif //GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
