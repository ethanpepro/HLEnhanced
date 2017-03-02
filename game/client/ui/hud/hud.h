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

#include "voice_status.h" // base voice handling clas

class CHud : public CBaseHud
{
public:
	DECLARE_CLASS( CHud, CBaseHud );

public:
	CHud();
	~CHud();			// destructor, frees allocated memory

	void CreateHudElements( CHudList& list ) override;

	void Init() override;
	void VidInit() override;
	void InitHud() override;
	void ResetHud() override;

protected:
	void LoadSprites() override;
	bool DoDraw( float flTime, bool intermission ) override;
	bool PreThinkUpdateClient( client_data_t* cdata ) override;
	bool PostThinkUpdateClient( client_data_t* cdata ) override;
	void Think() override;

public:
	/**
	*	@return The primary HUD color.
	*/
	const ::Color& GetPrimaryColor() const { return m_HudColors.m_PrimaryColor; }

	/**
	*	Sets the primary HUD color.
	*	@param color Color to set.
	*/
	void SetPrimaryColor( const ::Color& color )
	{
		m_HudColors.m_PrimaryColor = color;
	}

	/**
	*	@return The empty / nearly empty HUD color.
	*/
	const ::Color& GetEmptyItemColor() const { return m_HudColors.m_EmptyItemColor; }

	/**
	*	Sets the empty / nearly empty HUD color.
	*	@param color Color to set.
	*/
	void SetEmptyItemColor( const ::Color& color )
	{
		m_HudColors.m_EmptyItemColor = color;
	}

	/**
	*	@return The ammo bar HUD color.
	*/
	const ::Color& GetAmmoBarColor() const { return m_HudColors.m_AmmoBarColor; }

	/**
	*	Sets the ammo bar HUD color.
	*	@param color Color to set.
	*/
	void SetAmmoBarColor( const ::Color& color )
	{
		m_HudColors.m_AmmoBarColor = color;
	}

	int DrawHudNumber(int x, int y, int iFlags, int iNumber, int r, int g, int b );
	int DrawHudString(int x, int y, int iMaxX, char *szString, int r, int g, int b );
	int DrawHudStringReverse( int xpos, int ypos, int iMinX, char *szString, int r, int g, int b );
	int DrawHudNumberString( int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b );
	int GetNumWidth(int iNumber, int iFlags);

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

public:
	cvar_t*		m_pCvarStealMouse;
	cvar_t*		m_pCvarDraw;

private:
	HSPRITE		m_hsprCursor = 0;
	HSPRITE		m_hsprLogo = 0;
	bool		m_bLogo = false;
	int			m_iConcussionEffect;

	CHudColors	m_HudColors;
};

extern CHud gHUD;

extern int g_iPlayerClass;
extern int g_iTeamNumber;
extern int g_iUser1;
extern int g_iUser2;
extern int g_iUser3;

#endif //GAME_CLIENT_UI_HUD_HUD_H
