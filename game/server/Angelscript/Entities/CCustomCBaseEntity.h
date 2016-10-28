#ifndef GAME_SERVER_ANGELSCRIPT_ENTITIES_CCUSTOMCBASEENTITY_H
#define GAME_SERVER_ANGELSCRIPT_ENTITIES_CCUSTOMCBASEENTITY_H

#include <Angelscript/util/CASObjPtr.h>
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

	CASObjPtr& GetObject() override final
	{
		return CBaseCustomCBaseEntity::m_Instance;
	}

	void SetObject( CASObjPtr&& instance ) override final
	{
		CBaseCustomCBaseEntity::m_Instance = std::move( instance );
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

private:
	CASObjPtr m_Instance;
};

/**
*	C++ Class type for custom CBaseEntity entities.
*/
using CCustomCBaseEntity = CBaseCustomCBaseEntity<CBaseEntity>;

#endif //GAME_SERVER_ANGELSCRIPT_ENTITIES_CCUSTOMCBASEENTITY_H
