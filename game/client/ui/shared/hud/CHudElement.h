#ifndef GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENT_H
#define GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENT_H

#include "shared_game_utils.h"
#include "entities/DataMapping.h"

/**
*	Base class for Hud elements.
*/
class CHudElement
{
public:
	DECLARE_CLASS_NOBASE( CHudElement );

public:
	/**
	*	@param pszName The name of this Hud element.
	*/
	CHudElement( const char* const pszName );
	virtual ~CHudElement();

	/**
	*	@return The name of this Hud element.
	*/
	const char* GetName() const { return m_pszName; }

private:
	const char* const m_pszName;

private:
	CHudElement( const CHudElement& ) = delete;
	CHudElement& operator=( const CHudElement& ) = delete;
};

#endif //GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENT_H
