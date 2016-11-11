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

#include "CBitSet.h"

/**
*	Base class for all HUD classes.
*/
class CHudBase
{
public:
	using Flags_t = CBitSet<int>;

public: 
	virtual	~CHudBase() {}

	/**
	*	@return This Hud element's flags.
	*/
	const Flags_t& GetFlags() const { return m_Flags; }

	/**
	*	@copydoc GetFlags() const
	*/
	Flags_t& GetFlags() { return m_Flags; }

	/**
	*	Called when the client is initializing.
	*	@return true on success, false otherwise. Ignored.
	*/
	virtual bool Init() { return false; }

	/**
	*	Called after a connection to a server has been established.
	*	@return true on success, false otherwise. Ignored.
	*/
	virtual bool VidInit() { return false; }

	/**
	*	Step through the local data,  placing the appropriate graphics & text as appropriate
	*	@param flTime Current time.
	*	@return true if they've changed, false otherwise
	*/
	virtual bool Draw( float flTime ) { return false; }

	/**
	*	Called every time shared client dll/engine data gets changed.
	*/
	virtual void Think() {}

	/**
	*	Called when the HUD needs to be reset.
	*/
	virtual void Reset() {}

	/**
	*	Called every time a server is connected to.
	*/
	virtual void InitHUDData() {}

private:
	// active, moving,
	Flags_t m_Flags;
};

#endif //GAME_CLIENT_UI_HUD_CHUDBASE_H
