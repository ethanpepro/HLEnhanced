#ifndef GAME_SERVER_ANGELSCRIPT_ENTITIES_IASCUSTOMENTITY_H
#define GAME_SERVER_ANGELSCRIPT_ENTITIES_IASCUSTOMENTITY_H

#include <angelscript.h>

#include <Angelscript/util/CASObjPtr.h>

#undef GetObject

/**
*	Interface for custom entity classes.
*/
class IASCustomEntity
{
public:
	virtual ~IASCustomEntity() = 0;

	/**
	*	@return The object pointer for this custom entity's script object.
	*/
	virtual CASObjPtr& GetObject() = 0;

	/**
	*	Sets this entity's script object.
	*/
	virtual void SetObject( CASObjPtr&& instance ) = 0;

	/*
	*	Called when the script instance has fully initialized.
	*	Use this to communicate with the script instance, and decide if you're in a proper state.
	*	Return false if you want to be destroyed.
	*/
	virtual bool OnScriptClassReady();
};

inline IASCustomEntity::~IASCustomEntity()
{
}

inline bool IASCustomEntity::OnScriptClassReady()
{
	return true;
}

#endif //GAME_SERVER_ANGELSCRIPT_ENTITIES_IASCUSTOMENTITY_H
