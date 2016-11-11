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
#ifndef GAME_CLIENT_UI_HUD_CHUDAMMOSECONDARY_H
#define GAME_CLIENT_UI_HUD_CHUDAMMOSECONDARY_H

class CHudAmmoSecondary : public CHudBase
{
public:
	DECLARE_CLASS( CHudAmmoSecondary, CHudBase );

	CHudAmmoSecondary( const char* const pszName );

	bool Init() override;
	bool VidInit() override;
	void Reset() override;
	bool Draw( float flTime ) override;

	int MsgFunc_SecAmmoVal( const char *pszName, int iSize, void *pbuf );
	int MsgFunc_SecAmmoIcon( const char *pszName, int iSize, void *pbuf );

private:
	enum {
		MAX_SEC_AMMO_VALUES = 4
	};

	int m_HUD_ammoicon; // sprite indices
	int m_iAmmoAmounts[ MAX_SEC_AMMO_VALUES ];
	float m_fFade;
};

#endif //GAME_CLIENT_UI_HUD_CHUDAMMOSECONDARY_H
