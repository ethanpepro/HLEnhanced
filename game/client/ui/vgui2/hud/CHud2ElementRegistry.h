#ifndef GAME_CLIENT_UI_VGUI2_HUD_CHUD2ELEMENTREGISTRY_H
#define GAME_CLIENT_UI_VGUI2_HUD_CHUD2ELEMENTREGISTRY_H

class CHud2Element;
class CBaseHud2;
struct CHud2DefaultableArgs;

/**
*	Class used to create a static list of Hud element classes.
*	The list is sorted from deepest to shallowest element.
*/
class CHud2ElementRegistry final
{
public:
	using CreateFn = CHud2Element* ( * )( );

public:
	/**
	*	@param createFn Function used to create the element.
	*	@param args Defaultable arguments.
	*	@see CHud2DefaultableArgs
	*/
	CHud2ElementRegistry( CreateFn createFn, const CHud2DefaultableArgs& args );

	static CHud2ElementRegistry* GetHead() { return m_pHead; }

	CHud2ElementRegistry* GetNext() { return m_pNext; }

	CreateFn GetCreateFunction() { return m_CreateFn; }

	/**
	*	Creates all elements and adds them to the given Hud.
	*	@return Number of elements created.
	*/
	static size_t CreateAllElements( CBaseHud2& hud );

private:
	static CHud2ElementRegistry* m_pHead;

	CHud2ElementRegistry* m_pNext;

	CreateFn m_CreateFn;

	const int m_iDepth;

private:
	CHud2ElementRegistry( const CHud2ElementRegistry& ) = delete;
	CHud2ElementRegistry& operator=( const CHud2ElementRegistry& ) = delete;
};

/**
*	Helper struct to give default arguments to the Hud element registry.
*/
struct CHud2DefaultableArgs final
{
	/**
	*	First parameter is a dummy so this works on Linux, since pasting varargs to '(' doesn't work.
	*	@param iDepth Depth of the element in the Hud element tree. Lower values render on top of higher values. [ 1, 100 ].
	*/
	CHud2DefaultableArgs( const int, const int iDepth = 50 )
		: m_iDepth( iDepth )
	{
	}

	const int m_iDepth;
};

#define REGISTER_NAMED_HUD2ELEMENT( className, name, ... )																		\
static CHud2Element* __Create##name##()																							\
{																																\
	return new className( #name );																								\
}																																\
																																\
static CHud2ElementRegistry __g_Hud2Element##name##Registry( &__Create##name, CHud2DefaultableArgs( 0, ##__VA_ARGS__ ) )

#define REGISTER_HUD2ELEMENT( className, ... )						\
REGISTER_NAMED_HUD2ELEMENT( className, className, ##__VA_ARGS__ )

#endif //GAME_CLIENT_UI_VGUI2_HUD_CHUD2ELEMENTREGISTRY_H
