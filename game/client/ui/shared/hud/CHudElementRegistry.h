#ifndef GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENTREGISTRY_H
#define GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENTREGISTRY_H

#include "CHudElement.h"

class CHudList;
struct CHudDefaultableArgs;
class CHudElementRegistry;

const int HUD_ELEMENT_DEPTH_MIN = 1;
const int HUD_ELEMENT_DEPTH_MAX = 100;
const int HUD_ELEMENT_DEPTH_DEFAULT = 50;

/**
*	Represents one Hud element in a registry.
*/
class CHudElementRegistryEntry final
{
public:
	using CreateFn = CHudElement* ( * )();

public:
	/**
	*	@param registry Registry of Hud elements to add this entry to
	*	@param createFn Function used to create the element
	*	@param args Defaultable arguments
	*	@see CHudDefaultableArgs
	*/
	CHudElementRegistryEntry( CHudElementRegistry& registry, CreateFn createFn, const CHudDefaultableArgs& args );

	CHudElementRegistryEntry* GetNext() { return m_pNext; }

	CreateFn GetCreateFunction() { return m_CreateFn; }

	int GetDepth() const { return m_iDepth; }

private:
	CHudElementRegistryEntry* m_pNext;

	CreateFn m_CreateFn;

	const int m_iDepth;

private:
	CHudElementRegistryEntry( const CHudElementRegistryEntry& ) = delete;
	CHudElementRegistryEntry& operator=( const CHudElementRegistryEntry& ) = delete;
};

/**
*	Stores a list of Hud element factories to be used to populate a Hud.
*	The list is sorted from deepest to shallowest element.
*/
class CHudElementRegistry final
{
public:
	friend class CHudElementRegistryEntry;

public:
	/**
	*	@param pszName Name of this registry
	*/
	CHudElementRegistry( const char* pszName );
	~CHudElementRegistry() = default;

	/**
	*	@return The name of this registry
	*/
	const char* GetName() const { return m_pszName; }

	/**
	*	@return The first registry entry in the list
	*/
	CHudElementRegistryEntry* GetHead() { return m_pHead; }

	/**
	*	Creates all elements and adds them to the given Hud.
	*	@return Number of elements created
	*/
	size_t CreateAllElements( CHudList& hudList );

private:
	const char* const m_pszName;
	CHudElementRegistryEntry* m_pHead;

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
	*	@param iDepth Depth of the element in the Hud element tree. Lower values render on top of higher values. [ 1, 100 ]
	*/
	CHudDefaultableArgs( const int, const int iDepth = HUD_ELEMENT_DEPTH_DEFAULT )
		: m_iDepth( iDepth )
	{
	}

	const int m_iDepth;
};

/**
*	Declares a function "CHudElementRegistry& name()" that returns a registry.
*/
#define DECLARE_HUDELEMENT_REGISTRY( name )	\
CHudElementRegistry& name()

/**
*	Implements a Hud element registry accessor.
*/
#define IMPLEMENT_HUDELEMENT_REGISTRY( name )		\
DECLARE_HUDELEMENT_REGISTRY( name )					\
{													\
	static CHudElementRegistry instance( #name );	\
													\
	return instance;								\
}

/**
*	Registers a named Hud element with the given registry.
*	@param registryName Name of the registry to add this to. Must be declared with DECLARE_HUDELEMENT_REGISTRY before you can reference it
*	@param className Name of the class being registered
*	@param name Name that this element can be referenced by
*	@param ... Optional arguments
*	@see CHudDefaultableArgs for optional arguments
*/
#define REGISTER_NAMED_HUDELEMENT( registryName, className, name, ... )																		\
static CHudElement* __Create##name()																										\
{																																			\
	return new className( #name );																											\
}																																			\
																																			\
static CHudElementRegistryEntry __g_HudElement##name##Entry( registryName(), &__Create##name, CHudDefaultableArgs( 0, ##__VA_ARGS__ ) )

/**
*	@see REGISTER_NAMED_HUDELEMENT
*/
#define REGISTER_HUDELEMENT( registryName, className, ... )						\
REGISTER_NAMED_HUDELEMENT( registryName, className, className, ##__VA_ARGS__ )

#endif //GAME_CLIENT_UI_SHARED_HUD_CHUDELEMENTREGISTRY_H
