#ifndef GAME_CLIENT_UI_VGUI2_HUD_CHUD2ELEMENT_H
#define GAME_CLIENT_UI_VGUI2_HUD_CHUD2ELEMENT_H

/**
*	Base class for VGUI2 Hud elements.
*/
class CHud2Element
{
public:
	/**
	*	@param pszName The name of this Hud element.
	*/
	CHud2Element( const char* const pszName );
	virtual ~CHud2Element();

	/**
	*	@return The name of this Hud element.
	*/
	const char* GetName() const { return m_pszName; }

private:
	const char* const m_pszName;

private:
	CHud2Element( const CHud2Element& ) = delete;
	CHud2Element& operator=( const CHud2Element& ) = delete;
};

#endif //GAME_CLIENT_UI_VGUI2_HUD_CHUD2ELEMENT_H
