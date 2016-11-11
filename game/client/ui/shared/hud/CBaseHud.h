#ifndef GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
#define GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H

#include <utlvector.h>

#include "shared_game_utils.h"
//TODO: need to move DataMapping out of entities. - Solokiller
#include "entities/DataMapping.h"

class CHudElement;

/**
*	Base class for the Hud.
*/
class CBaseHud
{
public:
	DECLARE_CLASS_NOBASE( CBaseHud );

public:
	CBaseHud();
	virtual ~CBaseHud();

	//Element list

	/**
	*	@return The number of elements in the Hud.
	*/
	int GetElementCount() const;

	/**
	*	Gets a Hud element by index. Must be in the range [ 0, GetElementCount [.
	*/
	CHudElement* GetElementByIndex( int iIndex );

	/**
	*	Gets an element by name.
	*/
	CHudElement* GetElementByName( const char* const pszName );

	/**
	*	Adds a new Hud element.
	*	@return Whether the element was added to the Hud.
	*/
	bool AddElement( CHudElement* pElement );

	/**
	*	Removes an element from the Hud.
	*	@param bDelete Whether to also delete the element.
	*/
	void RemoveElement( CHudElement* pElement, const bool bDelete = true );

	/**
	*	Removes all elements from the Hud.
	*	@param bDelete Whether to also delete all elements.
	*/
	void RemoveAllElements( const bool bDelete = true );

private:
	CUtlVector<CHudElement*> m_Elements;

private:
	CBaseHud( const CBaseHud& ) = delete;
	CBaseHud& operator=( const CBaseHud& ) = delete;
};

#endif //GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
