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
#ifndef GAME_CLIENT_UI_HUD_CHUDBASE_H
#define GAME_CLIENT_UI_HUD_CHUDBASE_H

//Probably duplicate somewhere. TODO - Solokiller
struct POSITION
{
	int x, y;
};

class CHudBase
{
public:
	POSITION  m_pos;
	int   m_type;
	int	  m_iFlags; // active, moving, 
	virtual	~CHudBase() {}
	virtual bool Init() { return false; }
	virtual bool VidInit() { return false; }
	virtual bool Draw( float flTime ) { return false; }
	virtual void Think() {}
	virtual void Reset() {}
	virtual void InitHUDData() {}		// called every time a server is connected to
};

#endif //GAME_CLIENT_UI_HUD_CHUDBASE_H
