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
protected:
	/**
	*	Callback used to evaluate whether an element should be drawn.
	*	@return true if the element should be drawn
	*/
	using HudElementEvaluatorFn = bool ( * )( CHudElement* pElement, void* pUserData );

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

	bool Redraw( float flTime, bool intermission );

protected:
	/**
	*	Subclasses should override this to handle specific drawing needs.
	*/
	virtual bool DoDraw( float flTime, bool intermission ) = 0;

	/**
	*	Draws all Hud elements.
	*	@param flTime Current time
	*	@param evaluatorFn Function to evaluate if an element should be drawn
	*	@param pUserData Optional user data to pass to the evaluator function
	*/
	void DrawHudElements( float flTime, HudElementEvaluatorFn evaluatorFn, void* pUserData = nullptr );

public:
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

	float GetTime() const { return m_flTime; }

	float GetOldTime() const { return m_flOldTime; }

	double GetTimeDelta() const { return m_flTimeDelta; }

	bool IsInIntermission() const { return m_bIntermission; }

private:
	float m_flTime			=	1.f;	// the current client time
	float m_flOldTime		=	0;		// the time at which the HUD was last redrawn
	double m_flTimeDelta	=	0;		// the difference between flTime and fOldTime

	bool m_bIntermission = false;

	float m_flSnapshotTime = 0;			//! If non-zero, the time at which to take a snapshot.

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
