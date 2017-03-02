#ifndef GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
#define GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H

#include "shared_game_utils.h"
//TODO: need to move DataMapping out of entities. - Solokiller
#include "entities/DataMapping.h"

#include "CHudList.h"
#include "HudDefs.h"

#include "CBitSet.h"

/**
*	Helper macro to easily create Hud elements.
*/
#define CREATE_HUDELEMENT( name )	\
new name( #name )

/**
*	Data for a single Hud sprite.
*/
struct HudSprite_t
{
	HSPRITE hSprite;
	wrect_t rect;
	char szName[ MAX_SPRITE_NAME_LENGTH ];
};

/**
*	Base class for the Hud.
*/
class CBaseHud
{
public:
	static const int INVALID_SPRITE_INDEX = -1;

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
	*	Creates the Hud elements for this Hud.
	*/
	virtual void CreateHudElements( CHudList& list ) {}

	virtual void Init();

	virtual void VidInit();

protected:
	/**
	*	Called when the Hud can load its sprites.
	*/
	virtual void LoadSprites() {}

public:
	virtual void ResetHud();

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
	/**
	*	Updates the FOV. Can be overridden for custom handling.
	*	@param iNewFOV New Field Of Fiew
	*	@param bForce Whether to force the FOV to this setting
	*/
	virtual void UpdateFOV( int iNewFOV, bool bForce );

	virtual void InitHudElements();

	/**
	*	Called when the game is being shut down.
	*/
	virtual void GameShutdown();

	float GetTime() const { return m_flTime; }

	float GetOldTime() const { return m_flOldTime; }

	double GetTimeDelta() const { return m_flTimeDelta; }

	const SCREENINFO& ScreenInfo() const { return m_scrinfo; }

	int GetFOV() const { return m_iFOV; }

	void SetFOV( int iFOV )
	{
		m_iFOV = iFOV;
	}

	float GetSensitivity() const
	{
		return m_flMouseSensitivity;
	}

	void SetSensitivity( float flMouseSensitivity )
	{
		m_flMouseSensitivity = flMouseSensitivity;
	}

	int GetResolution() const { return m_iResolution; }

	bool IsInIntermission() const { return m_bIntermission; }

	const Vector& GetOrigin() const { return m_vecOrigin; }

	const Vector& GetAngles() const { return m_vecAngles; }

	int GetKeyBits() const { return m_iKeyBits; }

	void ClearKeyBits( int bits )
	{
		m_iKeyBits &= ~bits;
	}

	int GetWeaponBits() const { return m_iWeaponBits; }

	bool IsTeamplay() const { return m_bIsTeamplay; }

	void SetIsTeamplay( bool bIsTeamplay )
	{
		m_bIsTeamplay = bIsTeamplay;
	}

	const CBitSet<int>& GetHideHudBits() const { return m_HideHUDDisplay; }

	CBitSet<int>& GetHideHudBits() { return m_HideHUDDisplay; }

	HSPRITE GetSprite( int index ) const;

	const wrect_t& GetSpriteRect( int index ) const;

	/**
	*	Searches through the sprite list loaded from hud.txt for a name matching SpriteName
	*	returns an index into the gHUD.m_rghSprites[] array
	*	returns INVALID_SPRITE_INDEX if sprite not found
	*/
	int GetSpriteIndex( const char* SpriteName ) const;

	cvar_t* GetDefaultFOVCVar() { return default_fov; }

private:
	float m_flTime			=	1.f;	// the current client time
	float m_flOldTime		=	0;		// the time at which the HUD was last redrawn
	double m_flTimeDelta	=	0;		// the difference between flTime and fOldTime

	float m_flSnapshotTime = 0;			//! If non-zero, the time at which to take a snapshot.

	SCREENINFO m_scrinfo;				// Screen information

	int	m_iFOV = 0;
	float m_flMouseSensitivity = 0;

	int m_iResolution = 0;

	//Game state
	bool			m_bIntermission = false;

	//TODO: these should probably be taken from the local CBasePlayer - Solokiller
	Vector			m_vecOrigin;
	Vector			m_vecAngles;

	int				m_iKeyBits = 0;
	int				m_iWeaponBits = 0;

	bool			m_bIsTeamplay = false;

	//Hud element state.
	CBitSet<int>	m_HideHUDDisplay;

	//Hud sprites
	// the memory for these arrays are allocated in the first call to CBaseHud::VidInit(), when the hud.txt and associated sprites are loaded.
	// freed in ~CBaseHud()
	client_sprite_t*	m_pSpriteList = nullptr;
	int					m_iSpriteCount = 0;
	int					m_iSpriteCountAllRes = 0;	//!Sprite count for all resolutions.

	//Only one array now to make things a little saner.
	HudSprite_t* m_pSprites = nullptr;	// the sprites loaded from hud.txt, total m_iSpriteCount

	//CVars
	cvar_t* default_fov = nullptr;

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
