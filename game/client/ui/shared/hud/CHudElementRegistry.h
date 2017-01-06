#ifndef GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENTREGISTRY_H
#define GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENTREGISTRY_H

#include "CHudElement.h"

class CBaseHud;
struct CHudDefaultableArgs;

const int HUD_DEFAULT_DEPTH = 50;

/**
*	Class used to create a static list of Hud element classes.
*	The list is sorted from deepest to shallowest element.
*/
class CHudElementRegistry final
{
public:
	using CreateFn = CHudElement* ( * )( );

public:
	/**
	*	@param createFn Function used to create the element.
	*	@param args Defaultable arguments.
	*	@see CHud2DefaultableArgs
	*/
	CHudElementRegistry( CreateFn createFn, const CHudDefaultableArgs& args );

	static CHudElementRegistry* GetHead() { return m_pHead; }

	CHudElementRegistry* GetNext() { return m_pNext; }

	CreateFn GetCreateFunction() { return m_CreateFn; }

	/**
	*	Creates all elements and adds them to the given Hud.
	*	@return Number of elements created.
	*/
	static size_t CreateAllElements( CBaseHud& hud );

private:
	static CHudElementRegistry* m_pHead;

	CHudElementRegistry* m_pNext;

	CreateFn m_CreateFn;

	const int m_iDepth;

private:
	CHudElementRegistry( const CHudElementRegistry& ) = delete;
	CHudElementRegistry& operator=( const CHudElementRegistry& ) = delete;
};

/**
*	Helper struct to give default arguments to the Hud element registry.
*/
struct CHudDefaultableArgs final
{
	/**
	*	First parameter is a dummy so this works on Linux, since pasting varargs to '(' doesn't work.
	*	@param iDepth Depth of the element in the Hud element tree. Lower values render on top of higher values. [ 1, 100 ].
	*/
	CHudDefaultableArgs( const int, const int iDepth = HUD_DEFAULT_DEPTH )
		: m_iDepth( iDepth )
	{
	}

	const int m_iDepth;
};

#define REGISTER_NAMED_HUDELEMENT( className, name, ... )																		\
static CHudElement* __Create##name()																							\
{																																\
	return new className( #name );																								\
}																																\
																																\
static CHudElementRegistry __g_HudElement##name##Registry( &__Create##name, CHudDefaultableArgs( 0, ##__VA_ARGS__ ) )

#define REGISTER_HUDELEMENT( className, ... )						\
REGISTER_NAMED_HUDELEMENT( className, className, ##__VA_ARGS__ )

#endif //GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENTREGISTRY_H
