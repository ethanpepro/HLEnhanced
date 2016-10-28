#ifndef GAME_SERVER_ANGELSCRIPT_ENTITIES_IASCUSTOMENTITY_H
#define GAME_SERVER_ANGELSCRIPT_ENTITIES_IASCUSTOMENTITY_H

#include <angelscript.h>

#include <Angelscript/util/CASObjPtr.h>

#undef GetObject

class CASCustomEntities;
class CASCustomEntities::CCustomEntityClass;

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
	virtual CASObjPtr& GetObject() const = 0;

	/**
	*	Sets this entity's script object.
	*/
	virtual void SetObject( CASObjPtr&& instance ) = 0;

	/**
	*	Sets the class object.
	*/
	virtual void SetClass( const CASCustomEntities::CCustomEntityClass* pClass ) = 0;

	/**
	*	Sets this entity's Think function.
	*/
	virtual void SetScriptThink( asIScriptFunction* pFunction ) = 0;

	/**
	*	Sets this entity's Touch function.
	*/
	virtual void SetScriptTouch( asIScriptFunction* pFunction ) = 0;

	/**
	*	Sets this entity's Use function.
	*/
	virtual void SetScriptUse( asIScriptFunction* pFunction ) = 0;

	/**
	*	Sets this entity's Blocked function.
	*/
	virtual void SetScriptBlocked( asIScriptFunction* pFunction ) = 0;

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
