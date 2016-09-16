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

#include "Color.h"
#include "HudColors.h"

#ifndef _WIN32
#define _cdecl 
#endif

#include "wrect.h"
#include "cl_dll.h"

#include "global_consts.h"

#define DHN_DRAWZERO 1
#define DHN_2DIGITS  2
#define DHN_3DIGITS  4
#define MIN_ALPHA	 100	

//TODO: identical to PackedColorVec - Solokiller
//TODO: used in only one place. Can replace with HudColor once it has alpha channel support - Solokiller
struct RGBA
{
	unsigned char r,g,b,a;
};

struct cvar_t;

enum HudFlag
{
	HUD_ACTIVE			= 1,
	HUD_INTERMISSION	= 2,
};

class CHudBase;

struct HUDLIST
{
	CHudBase	*p;
	HUDLIST		*pNext;
};

struct extra_player_info_t
{
	short frags;
	short deaths;
	short playerclass;
	short health; // UNUSED currently, spectator UI would like this
	bool dead; // UNUSED currently, spectator UI would like this
	short teamnumber;
	char teamname[ MAX_TEAM_NAME ];
};

struct team_info_t
{
	char name[ MAX_TEAM_NAME ];
	short frags;
	short deaths;
	short ping;
	short packetloss;
	short ownteam;
	short players;
	bool already_drawn;
	bool scores_overriden;
	int teamnumber;
};

struct message_parms_t
{
	client_textmessage_t	*pMessage;
	float	time;
	int x, y;
	int	totalWidth, totalHeight;
	int width;
	int lines;
	int lineLength;
	int length;
	int r, g, b;
	int text;
	int fadeBlend;
	float charTime;
	float fadeTime;
};

#include "player_info.h"

//TODO: move - Solokiller
#define	MAX_MOTD_LENGTH				1536

#define FADE_TIME 100

#define MAX_SPRITE_NAME_LENGTH	24

const int maxHUDMessages = 16;

#include "CHudBase.h"

#include "voice_status.h" // base voice handling class

#include "CHudAmmo.h"
#include "CHudAmmoSecondary.h"
#include "CHudBattery.h"
#include "CHudBenchmark.h"
#include "CHudDeathNotice.h"
#include "CHudFlashlight.h"
#include "CHudGeiger.h"
#include "CHudHealth.h"
#include "CHudMenu.h"
#include "CHudMessage.h"
#include "CHudSayText.h"
#include "CHudSpectator.h"
#include "CHudStatusBar.h"
#include "CHudStatusIcons.h"
#include "CHudTextMessage.h"
#include "CHudTrain.h"

class CHud
{
private:
	HUDLIST						*m_pHudList;
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

	CHudAmmo			m_Ammo;
	CHudHealth			m_Health;
	CHudSpectator		m_Spectator;
	CHudGeiger			m_Geiger;
	CHudBattery			m_Battery;
	CHudTrain			m_Train;
	CHudFlashlight		m_Flash;
	CHudMessage			m_Message;
	CHudStatusBar		m_StatusBar;
	CHudDeathNotice		m_DeathNotice;
	CHudSayText			m_SayText;
	CHudMenu			m_Menu;
	CHudAmmoSecondary	m_AmmoSecondary;
	CHudTextMessage		m_TextMessage;
	CHudStatusIcons		m_StatusIcons;
	CHudBenchmark		m_Benchmark;

	void Init();
	void VidInit();
	void Think();
	bool Redraw( float flTime, int intermission );
	int UpdateClientData( client_data_t *cdata, float time );

	CHud() : m_iSpriteCount(0), m_pHudList(NULL) {}  
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


	void AddHudElem( CHudBase* pHudElem );

	float GetSensitivity();

private:
	void FreeHudList();
};

extern CHud gHUD;

extern int g_iPlayerClass;
extern int g_iTeamNumber;
extern int g_iUser1;
extern int g_iUser2;
extern int g_iUser3;

const Vector& GetClientColor( int clientIndex );
extern const Vector g_ColorYellow;

#endif //GAME_CLIENT_UI_HUD_HUD_H