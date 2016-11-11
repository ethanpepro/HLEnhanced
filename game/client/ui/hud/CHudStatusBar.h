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
#ifndef GAME_CLIENT_UI_HUD_CHUDSTATUSBAR_H
#define GAME_CLIENT_UI_HUD_CHUDSTATUSBAR_H

class CHudStatusBar : public CHudBase
{
public:
	DECLARE_CLASS( CHudStatusBar, CHudBase );

	CHudStatusBar( const char* const pszName );

	bool Init() override;
	bool VidInit() override;
	bool Draw( float flTime );
	void Reset();
	void ParseStatusString( int line_num );

	int MsgFunc_StatusText( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_StatusValue( const char *pszName, int iSize, void *pbuf );

protected:
	enum {
		MAX_STATUSTEXT_LENGTH = 128,
		MAX_STATUSBAR_VALUES = 8,
		MAX_STATUSBAR_LINES = 3,
	};

	char m_szStatusText[ MAX_STATUSBAR_LINES ][ MAX_STATUSTEXT_LENGTH ];  // a text string describing how the status bar is to be drawn
	char m_szStatusBar[ MAX_STATUSBAR_LINES ][ MAX_STATUSTEXT_LENGTH ];	// the constructed bar that is drawn
	int m_iStatusValues[ MAX_STATUSBAR_VALUES ];  // an array of values for use in the status bar

	bool m_bReparseString; // set to true whenever the m_szStatusBar needs to be recalculated

						   // an array of colors...one color for each line
	const Vector* m_pvecNameColors[ MAX_STATUSBAR_LINES ];
};

#endif //GAME_CLIENT_UI_HUD_CHUDSTATUSBAR_H