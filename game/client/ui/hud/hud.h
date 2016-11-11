/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//			
//  hud.h
//
// class CHud declaration
//
// CHud handles the message, calculation, and drawing the HUD
//
#ifndef GAME_CLIENT_UI_HUD_HUD_H
#define GAME_CLIENT_UI_HUD_HUD_H

#include "wrect.h"
#include "cl_dll.h"

#include "shared/hud/CBaseHud.h"

#include "HudDefs.h"

struct cvar_t;

#include "CHudBase.h"

#include "voice_status.h" // base voice handling class

class CHud : public CBaseHud
{
private:
	HSPRITE						m_hsprLogo;
	int							m_iLogo;
	client_sprite_t				*m_pSpriteList;
	int							m_iSpriteCount;
	int							m_iSpriteCountAllRes;
	float						m_flMouseSensitivity;
	int							m_iConcussionEffect; 

	CHudColors m_HudColors;

public:

	/**
	*	@return The primary HUD color.
	*/
	const Color& GetPrimaryColor() const { return m_HudColors.m_PrimaryColor; }

	/**
	*	Sets the primary HUD color.
	*	@param color Color to set.
	*/
	void SetPrimaryColor( const Color& color )
	{
		m_HudColors.m_PrimaryColor = color;
	}

	/**
	*	@return The empty / nearly empty HUD color.
	*/
	const Color& GetEmptyItemColor() const { return m_HudColors.m_EmptyItemColor; }

	/**
	*	Sets the empty / nearly empty HUD color.
	*	@param color Color to set.
	*/
	void SetEmptyItemColor( const Color& color )
	{
		m_HudColors.m_EmptyItemColor = color;
	}

	/**
	*	@return The ammo bar HUD color.
	*/
	const Color& GetAmmoBarColor() const { return m_HudColors.m_AmmoBarColor; }

	/**
	*	Sets the ammo bar HUD color.
	*	@param color Color to set.
	*/
	void SetAmmoBarColor( const Color& color )
	{
		m_HudColors.m_AmmoBarColor = color;
	}

	HSPRITE						m_hsprCursor;
	float m_flTime;	   // the current client time
	float m_fOldTime;  // the time at which the HUD was last redrawn
	double m_flTimeDelta; // the difference between flTime and fOldTime
	Vector	m_vecOrigin;
	Vector	m_vecAngles;
	int		m_iKeyBits;
	int		m_iHideHUDDisplay;
	int		m_iFOV;
	int		m_Teamplay;
	int		m_iRes;
	cvar_t  *m_pCvarStealMouse;
	cvar_t	*m_pCvarDraw;

	int m_iFontHeight;
	int DrawHudNumber(int x, int y, int iFlags, int iNumber, int r, int g, int b );
	int DrawHudString(int x, int y, int iMaxX, char *szString, int r, int g, int b );
	int DrawHudStringReverse( int xpos, int ypos, int iMinX, char *szString, int r, int g, int b );
	int DrawHudNumberString( int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b );
	int GetNumWidth(int iNumber, int iFlags);

private:
	// the memory for these arrays are allocated in the first call to CHud::VidInit(), when the hud.txt and associated sprites are loaded.
	// freed in ~CHud()
	HSPRITE *m_rghSprites;	/*[HUD_SPRITE_COUNT]*/			// the sprites loaded from hud.txt
	wrect_t *m_rgrcRects;	/*[HUD_SPRITE_COUNT]*/
	char *m_rgszSpriteNames; /*[HUD_SPRITE_COUNT][MAX_SPRITE_NAME_LENGTH]*/

	cvar_t *default_fov;
public:
	HSPRITE GetSprite( int index ) 
	{
		return (index < 0) ? 0 : m_rghSprites[index];
	}

	wrect_t& GetSpriteRect( int index )
	{
		return m_rgrcRects[index];
	}

	
	int GetSpriteIndex( const char *SpriteName );	// gets a sprite index, for use in the m_rghSprites[] array

	void Init();
	void VidInit();
	void Think();
	bool Redraw( float flTime, int intermission );
	int UpdateClientData( client_data_t *cdata, float time );

	CHud();
	~CHud();			// destructor, frees allocated memory

	void ResetHUD();

	// user messages
	int _cdecl MsgFunc_Damage(const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_Logo(const char *pszName,  int iSize, void *pbuf);
	int _cdecl MsgFunc_ResetHUD(const char *pszName,  int iSize, void *pbuf);
	void _cdecl MsgFunc_InitHUD( const char *pszName, int iSize, void *pbuf );
	void _cdecl MsgFunc_ViewMode( const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_SetFOV(const char *pszName,  int iSize, void *pbuf);
	int  _cdecl MsgFunc_Concuss( const char *pszName, int iSize, void *pbuf );
	int _cdecl MsgFunc_ReceiveW( const char* pszName, int iSize, void* pBuf );

	int MsgFunc_HudColors( const char* pszName, int iSize, void* pBuf );

	// Screen information
	SCREENINFO	m_scrinfo;

	int	m_iWeaponBits;
	bool m_bPlayerDead;
	int m_iIntermission;

	// sprite indexes
	int m_HUD_number_0;

	float GetSensitivity();

private:
	/**
	*	Calls a member function on all Hud elements.
	*	@param function Function to call.
	*	@param args Arguments to pass to the function.
	*	@tparam FUNC Pointer to member function type.
	*	@tparam ARGS Argument types.
	*/
	template<typename FUNC, typename... ARGS>
	void ForEachHudElem( FUNC function, ARGS&&... args )
	{
		auto count = GetElementCount();

		for( decltype( count ) index = 0; index < count; ++index )
		{
			auto pElem = static_cast<CHudBase*>( GetElementByIndex( index ) );

			( pElem->*function )( std::move( args )... );
		}
	}
};

extern CHud gHUD;

extern int g_iPlayerClass;
extern int g_iTeamNumber;
extern int g_iUser1;
extern int g_iUser2;
extern int g_iUser3;

#endif //GAME_CLIENT_UI_HUD_HUD_H
