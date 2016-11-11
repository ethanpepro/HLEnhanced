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
#ifndef GAME_CLIENT_UI_HUD_CHUDBATTERY_H
#define GAME_CLIENT_UI_HUD_CHUDBATTERY_H

class CHudBattery : public CHudElement
{
public:
	DECLARE_CLASS( CHudBattery, CHudElement );

	CHudBattery( const char* const pszName );

	bool Init()  override;
	bool VidInit() override;
	bool Draw( float flTime ) override;
	int MsgFunc_Battery( const char *pszName, int iSize, void *pbuf );

private:
	HSPRITE m_hSprite1;
	HSPRITE m_hSprite2;
	wrect_t *m_prc1;
	wrect_t *m_prc2;
	int	  m_iBat;
	int	  m_iBatMax;
	float m_fFade;
	int	  m_iHeight;		// width of the battery innards
};

#endif //GAME_CLIENT_UI_HUD_CHUDBATTERY_H