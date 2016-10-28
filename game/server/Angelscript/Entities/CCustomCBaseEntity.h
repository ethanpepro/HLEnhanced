#ifndef GAME_SERVER_ANGELSCRIPT_ENTITIES_CCUSTOMCBASEENTITY_H
#define GAME_SERVER_ANGELSCRIPT_ENTITIES_CCUSTOMCBASEENTITY_H

#include <angelscript.h>

#include <Angelscript/util/CASObjPtr.h>
#include <Angelscript/util/CASRefPtr.h>
#include <Angelscript/util/CASExtendAdapter.h>

#include "IASCustomEntity.h"

#include "CASCustomEntities.h"

#undef GetObject

/**
*	Extension class for custom entities inheriting from BASECLASS.
*	Provides extension methods for CBaseEntity.
*/
template<typename BASECLASS>
class CBaseCustomCBaseEntity : public BASECLASS, public IASCustomEntity
{
public:
	DECLARE_CLASS( CBaseCustomCBaseEntity, BASECLASS );

	CASObjPtr& GetObject() const override final
	{
		//This cast is needed because you can't return non-const references to class members.
		return const_cast<CBaseCustomCBaseEntity*>( this )->CBaseCustomCBaseEntity::m_Instance;
	}

	void SetObject( CASObjPtr&& instance ) override final
	{
		CBaseCustomCBaseEntity::m_Instance = std::move( instance );
	}

	void SetClass( const CASCustomEntities::CCustomEntityClass* pClass ) override final
	{
		m_pClass = pClass;
	}

	/**
	*	Sets a function. Can be either a function of the CBaseEntity class or the script class.
	*	@param function Function to set.
	*	@param pFunction Function to set. Can be null.
	*	@param pszName Name of the fundamental function being set.
	*/
	void SetFunction( CASRefPtr<asIScriptFunction>& function, asIScriptFunction* pFunction, const char* const pszName )
	{
		if( !pFunction )
		{
			function = nullptr;
			return;
		}

		bool bSuccess = false;

		if( pFunction->GetDelegateFunction() )
		{
			auto pDelegateType = pFunction->GetDelegateObjectType();

			ASSERT( pDelegateType );

			//Note: when checking for inheritance, check if our concrete type inherits from the given type.
			//When checking if our C++ type matches, check if the given type inherits from it.
			//This prevents subclasses from being
			const bool bIsScriptFunction = m_Instance.GetTypeInfo()->DerivesFrom( pDelegateType );

			auto pCPPType = pDelegateType->GetEngine()->GetTypeInfoByName( m_pClass->GetBaseClassData().szCPPClassName.c_str() );

			if( bIsScriptFunction ||
				pCPPType && pCPPType->DerivesFrom( pDelegateType ) )
			{
				bool bIsValid = false;

				if( bIsScriptFunction )
				{
					bIsValid = pFunction->GetDelegateObject() == m_Instance.Get();
				}
				else
				{
					bIsValid = pFunction->GetDelegateObject() == this;
				}

				if( bIsValid )
				{
					function = pFunction;

					bSuccess = true;
				}
			}
		}

		if( !bSuccess )
		{
			auto pActualFunction = pFunction->GetDelegateFunction();
			auto pActualObject = pFunction->GetDelegateObjectType();

			if( !pActualFunction )
				pActualFunction = pFunction;

			const std::string szCompleteName = pActualObject ? 
				std::string( pActualObject->GetNamespace() ) + "::" + pActualObject->GetName() + "::" + pActualFunction->GetName() : 
				std::string( pActualFunction->GetNamespace() ) + "::" + pActualFunction->GetName();

			Alert( at_warning, "Couldn't set custom entity \"%s\" (class %s) function %s to \"%s\"\n", 
				   GetTargetname(), GetClassname(), pszName, szCompleteName.c_str() );
		}

		pFunction->Release();
	}

	void OnCreate() override
	{
		CALL_EXTEND_FUNC( OnCreate, "()" );
	}

	void OnDestroy() override
	{
		CALL_EXTEND_FUNC( OnDestroy, "()" );

		m_Instance.Reset();
	}

	void UpdateOnRemove() override
	{
		CALL_EXTEND_FUNC( UpdateOnRemove, "()" );
	}

	void KeyValue( KeyValueData* pkvd ) override
	{
		CALL_EXTEND_FUNC( KeyValue, "(KeyValueData@)", pkvd );
	}

	void Precache() override
	{
		CALL_EXTEND_FUNC( Precache, "()" );
	}

	void Spawn() override
	{
		CALL_EXTEND_FUNC( Spawn, "()" );
	}

	void Activate() override
	{
		CALL_EXTEND_FUNC( Activate, "()" );
	}

	int ObjectCaps() const override
	{
		CALL_EXTEND_FUNC_RET( int, ObjectCaps, "()" );
	}

	void SetObjectCollisionBox() override
	{
		CALL_EXTEND_FUNC( SetObjectCollisionBox, "()" );
	}

	CBaseEntity* Respawn() override
	{
		//TODO: need helper macros for different return types.
		CBaseEntity* pResult = nullptr;

		if( auto pFunction = GetObject().GetTypeInfo()->GetMethodByDecl( "CBaseEntity@ Respawn()" ) )
		{
			CASOwningContext ctx( *pFunction->GetEngine() );

			CASMethod method( *pFunction, ctx, GetObject().Get() );

			if( method.Call( CallFlag::NONE ) )
			{
				method.GetReturnValue( &pResult );
			}
		}
		else
		{
			pResult = Respawn();
		}

		return pResult;
	}

	void DefaultScriptThink()
	{
		//Script didn't override Think, so call think function that was set, if any.

		if( m_ThinkFunc )
		{
			//This is a delegate so we don't pass 'this'.
			as::Call( m_ThinkFunc.Get() );
		}
	}

	void SetScriptThink( asIScriptFunction* pFunction ) override final
	{
		SetFunction( m_ThinkFunc, pFunction, "Think" );
	}

	void Think() override
	{
		CALL_EXTEND_FUNC_DIFFFUNC( Think, DefaultScriptThink, "()" );
	}

	void DefaultScriptTouch( CBaseEntity* pOther )
	{
		//Script didn't override Touch, so call think function that was set, if any.

		if( m_TouchFunc )
		{
			//This is a delegate so we don't pass 'this'.
			as::Call( m_TouchFunc.Get(), pOther );
		}
	}

	void SetScriptTouch( asIScriptFunction* pFunction ) override final
	{
		SetFunction( m_TouchFunc, pFunction, "Touch" );
	}

	void Touch( CBaseEntity* pOther ) override
	{
		CALL_EXTEND_FUNC_DIFFFUNC( Touch, DefaultScriptTouch, "(CBaseEntity@)", pOther );
	}

	void DefaultScriptUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue )
	{
		//Script didn't override Use, so call think function that was set, if any.

		if( m_UseFunc )
		{
			//This is a delegate so we don't pass 'this'.
			as::Call( m_UseFunc.Get(), pActivator, pCaller, useType, flValue );
		}
	}

	void SetScriptUse( asIScriptFunction* pFunction ) override final
	{
		SetFunction( m_UseFunc, pFunction, "Use" );
	}

	void Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue ) override
	{
		CALL_EXTEND_FUNC_DIFFFUNC( Use, DefaultScriptUse, "(CBaseEntity@, CBaseEntity@, USE_TYPE, float)", pActivator, pCaller, useType, flValue );
	}

	void DefaultScriptBlocked( CBaseEntity* pOther )
	{
		//Script didn't override Blocked, so call think function that was set, if any.

		if( m_BlockedFunc )
		{
			//This is a delegate so we don't pass 'this'.
			as::Call( m_BlockedFunc.Get(), pOther );
		}
	}

	void SetScriptBlocked( asIScriptFunction* pFunction ) override final
	{
		SetFunction( m_BlockedFunc, pFunction, "Blocked" );
	}

	void Blocked( CBaseEntity* pOther ) override
	{
		CALL_EXTEND_FUNC_DIFFFUNC( Blocked, DefaultScriptBlocked, "(CBaseEntity@)", pOther );
	}

private:
	CASObjPtr m_Instance;

	const CASCustomEntities::CCustomEntityClass* m_pClass = nullptr;

	CASRefPtr<asIScriptFunction> m_ThinkFunc;
	CASRefPtr<asIScriptFunction> m_TouchFunc;
	CASRefPtr<asIScriptFunction> m_UseFunc;
	CASRefPtr<asIScriptFunction> m_BlockedFunc;
};

/**
*	C++ Class type for custom CBaseEntity entities.
*/
using CCustomCBaseEntity = CBaseCustomCBaseEntity<CBaseEntity>;

#endif //GAME_SERVER_ANGELSCRIPT_ENTITIES_CCUSTOMCBASEENTITY_H
