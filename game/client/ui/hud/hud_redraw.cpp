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
// hud_redraw.cpp
//
#include <math.h>
#include "hud.h"
#include "cl_util.h"
#include "bench.h"
#include "strtools.h"

#include "vgui_TeamFortressViewport.h"

#define MAX_LOGO_FRAMES 56

int grgLogoFrame[MAX_LOGO_FRAMES] = 
{
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 13, 13, 13, 13, 13, 12, 11, 10, 9, 8, 14, 15,
	16, 17, 18, 19, 20, 20, 20, 20, 20, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
	29, 29, 29, 29, 29, 28, 27, 26, 25, 24, 30, 31 
};


extern int g_iVisibleMouse;

float HUD_GetFOV();

extern cvar_t *sensitivity;

// Think
void CHud::Think()
{
	m_scrinfo.iSize = sizeof(m_scrinfo);
	GetScreenInfo(&m_scrinfo);

	int newfov;

	auto count = HudList().GetElementCount();

	for( decltype( count ) index = 0; index < count; ++index )
	{
		auto pElem = HudList().GetElementByIndex( index );

		if( pElem->GetFlags() & HUD_ACTIVE )
			pElem->Think();
	}

	newfov = HUD_GetFOV();
	if ( newfov == 0 )
	{
		m_iFOV = default_fov->value;
	}
	else
	{
		m_iFOV = newfov;
	}

	// the clients fov is actually set in the client data update section of the hud

	// Set a new sensitivity
	if ( m_iFOV == default_fov->value )
	{  
		// reset to saved sensitivity
		m_flMouseSensitivity = 0;
	}
	else
	{  
		// set a new sensitivity that is proportional to the change from the FOV default
		m_flMouseSensitivity = sensitivity->value * ((float)newfov / (float)default_fov->value) * CVAR_GET_FLOAT("zoom_sensitivity_ratio");
	}

	// think about default fov
	if ( m_iFOV == 0 )
	{  // only let players adjust up in fov,  and only if they are not overriden by something else
		m_iFOV = max( default_fov->value, 90.0f );  
	}
	
	if ( gEngfuncs.IsSpectateOnly() )
	{
		if( auto pSpectator = GETHUDCLASS( CHudSpectator ) )
			m_iFOV = pSpectator->GetFOV();	// default_fov->value;
		else
			m_iFOV = default_fov->value;
	}

	Bench_CheckStart();
}

// Redraw
// step through the local data,  placing the appropriate graphics & text as appropriate
// returns true if they've changed, false otherwise
bool CHud::DoDraw( float flTime, bool intermission )
{
	// if no redrawing is necessary
	// return false;
	
	// draw all registered HUD elements
	if ( m_pCvarDraw->value )
	{
		struct CDrawInfo final
		{
			const bool bIntermission;
			CHud& hud;

			CDrawInfo( bool bIntermission, CHud& hud )
				: bIntermission( bIntermission )
				, hud( hud )
			{
			}
		};

		CDrawInfo info( intermission, *this );

		DrawHudElements(
			flTime,
			[]( CHudElement* pElem, void* pUserData )
			{
				const auto& info = *reinterpret_cast<const CDrawInfo*>( pUserData );

				if( !Bench_Active() )
				{
					if( !info.bIntermission )
					{
						if( ( pElem->GetFlags() & HUD_ACTIVE ) && !( info.hud.m_iHideHUDDisplay & HIDEHUD_ALL ) )
							return true;
					}
					else
					{  // it's an intermission,  so only draw hud elements that are set to draw during intermissions
						if( pElem->GetFlags() & HUD_INTERMISSION )
							return true;
					}
				}
				else
				{
					if( ( Q_strcmp( pElem->GetName(), "CHudBenckmark" ) == 0 ) &&
						( pElem->GetFlags() & HUD_ACTIVE ) &&
						!( info.hud.m_iHideHUDDisplay & HIDEHUD_ALL ) )
					{
						return true;
					}
				}

				return false;
			},
			&info
		);
	}

	// are we in demo mode? do we need to draw the logo in the top corner?
	if (m_iLogo)
	{
		int x, y, i;

		if (m_hsprLogo == 0)
			m_hsprLogo = LoadSprite("sprites/%d_logo.spr");

		SPR_Set(m_hsprLogo, 250, 250, 250 );
		
		x = SPR_Width(m_hsprLogo, 0);
		x = ScreenWidth - x;
		y = SPR_Height(m_hsprLogo, 0)/2;

		// Draw the logo at 20 fps
		int iFrame = (int)(flTime * 20) % MAX_LOGO_FRAMES;
		i = grgLogoFrame[iFrame] - 1;

		SPR_DrawAdditive(i, x, y, NULL);
	}

	/*
	if ( g_iVisibleMouse )
	{
		void IN_GetMousePos( int *mx, int *my );
		int mx, my;

		IN_GetMousePos( &mx, &my );
		
		if (m_hsprCursor == 0)
		{
			char sz[256];
			V_sprintf_safe( sz, "sprites/cursor.spr" );
			m_hsprCursor = SPR_Load( sz );
		}

		SPR_Set(m_hsprCursor, 250, 250, 250 );
		
		// Draw the logo at 20 fps
		SPR_DrawAdditive( 0, mx, my, NULL );
	}
	*/

	return true;
}

int CHud :: DrawHudString(int xpos, int ypos, int iMaxX, char *szIt, int r, int g, int b )
{
	return xpos + gEngfuncs.pfnDrawString( xpos, ypos, szIt, r, g, b);
}

int CHud :: DrawHudNumberString( int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b )
{
	char szString[32];
	V_sprintf_safe( szString, "%d", iNumber );
	return DrawHudStringReverse( xpos, ypos, iMinX, szString, r, g, b );

}

// draws a string from right to left (right-aligned)
int CHud :: DrawHudStringReverse( int xpos, int ypos, int iMinX, char *szString, int r, int g, int b )
{
	return xpos - gEngfuncs.pfnDrawStringReverse( xpos, ypos, szString, r, g, b);
}

int CHud :: DrawHudNumber( int x, int y, int iFlags, int iNumber, int r, int g, int b)
{
	int iWidth = GetSpriteRect(m_HUD_number_0).right - GetSpriteRect(m_HUD_number_0).left;
	int k;
	
	if (iNumber > 0)
	{
		// SPR_Draw 100's
		if (iNumber >= 100)
		{
			 k = iNumber/100;
			SPR_Set(GetSprite(m_HUD_number_0 + k), r, g, b );
			SPR_DrawAdditive( 0, x, y, &GetSpriteRect(m_HUD_number_0 + k));
			x += iWidth;
		}
		else if (iFlags & (DHN_3DIGITS))
		{
			//SPR_DrawAdditive( 0, x, y, &rc );
			x += iWidth;
		}

		// SPR_Draw 10's
		if (iNumber >= 10)
		{
			k = (iNumber % 100)/10;
			SPR_Set(GetSprite(m_HUD_number_0 + k), r, g, b );
			SPR_DrawAdditive( 0, x, y, &GetSpriteRect(m_HUD_number_0 + k));
			x += iWidth;
		}
		else if (iFlags & (DHN_3DIGITS | DHN_2DIGITS))
		{
			//SPR_DrawAdditive( 0, x, y, &rc );
			x += iWidth;
		}

		// SPR_Draw ones
		k = iNumber % 10;
		SPR_Set(GetSprite(m_HUD_number_0 + k), r, g, b );
		SPR_DrawAdditive(0,  x, y, &GetSpriteRect(m_HUD_number_0 + k));
		x += iWidth;
	} 
	else if (iFlags & DHN_DRAWZERO) 
	{
		SPR_Set(GetSprite(m_HUD_number_0), r, g, b );

		// SPR_Draw 100's
		if (iFlags & (DHN_3DIGITS))
		{
			//SPR_DrawAdditive( 0, x, y, &rc );
			x += iWidth;
		}

		if (iFlags & (DHN_3DIGITS | DHN_2DIGITS))
		{
			//SPR_DrawAdditive( 0, x, y, &rc );
			x += iWidth;
		}

		// SPR_Draw ones
		
		SPR_DrawAdditive( 0,  x, y, &GetSpriteRect(m_HUD_number_0));
		x += iWidth;
	}

	return x;
}


int CHud::GetNumWidth( int iNumber, int iFlags )
{
	if (iFlags & (DHN_3DIGITS))
		return 3;

	if (iFlags & (DHN_2DIGITS))
		return 2;

	if (iNumber <= 0)
	{
		if (iFlags & (DHN_DRAWZERO))
			return 1;
		else
			return 0;
	}

	if (iNumber < 10)
		return 1;

	if (iNumber < 100)
		return 2;

	return 3;

}	


