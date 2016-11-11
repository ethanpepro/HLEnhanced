#ifndef GAME_CLIENT_UI_VGUI2_HUD_CBASEHUD2_H
#define GAME_CLIENT_UI_VGUI2_HUD_CBASEHUD2_H

#include <utlvector.h>

class CHud2Element;

/**
*	Base class for the Hud.
*/
class CBaseHud2
{
public:
	CBaseHud2();
	virtual ~CBaseHud2();

	//Element list

	/**
	*	@return The number of elements in the Hud.
	*/
	int GetElementCount() const;

	/**
	*	Gets a Hud element by index. Must be in the range [ 0, GetElementCount [.
	*/
	CHud2Element* GetElementByIndex( int iIndex );

	/**
	*	Gets an element by name.
	*/
	CHud2Element* GetElementByName( const char* const pszName );

	/**
	*	Adds a new Hud element.
	*	@return Whether the element was added to the Hud.
	*/
	bool AddElement( CHud2Element* pElement );

	/**
	*	Removes an element from the Hud.
	*	@param bDelete Whether to also delete the element.
	*/
	void RemoveElement( CHud2Element* pElement, const bool bDelete = true );

	/**
	*	Removes all elements from the Hud.
	*	@param bDelete Whether to also delete all elements.
	*/
	void RemoveAllElements( const bool bDelete = true );

private:
	CUtlVector<CHud2Element*> m_Elements;

private:
	CBaseHud2( const CBaseHud2& ) = delete;
	CBaseHud2& operator=( const CBaseHud2& ) = delete;
};

#endif //GAME_CLIENT_UI_VGUI2_HUD_CBASEHUD2_H
