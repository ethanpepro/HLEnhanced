#ifndef GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
#define GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H

#include <UtlVector.h>

#include "shared_game_utils.h"
//TODO: need to move DataMapping out of entities. - Solokiller
#include "entities/DataMapping.h"

#include "shared/hud/CHudElement.h"

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

	virtual void InitHUDElements();

	/**
	*	Called when the game is being shut down.
	*/
	virtual void GameShutdown();

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

protected:
	/**
	*	Calls a member function on all Hud elements.
	*	@param function Function to call.
	*	@param args Arguments to pass to the function.
	*	@tparam FUNC Pointer to member function type.
	*	@tparam ARGS Argument types.
	*/
	template<typename FUNC, typename... ARGS>
	void ForEachHudElem( FUNC function, ARGS&&... args )
	{
		auto count = GetElementCount();

		for( decltype( count ) index = 0; index < count; ++index )
		{
			auto pElem = GetElementByIndex( index );

			( pElem->*function )( std::move( args )... );
		}
	}

protected:
	CUtlVector<CHudElement*> m_Elements;

	bool m_bGameShutdown = false;

private:
	CBaseHud( const CBaseHud& ) = delete;
	CBaseHud& operator=( const CBaseHud& ) = delete;
};

#endif //GAME_CLIENT_UI_SHARED_HUD_CBASEHUD_H
