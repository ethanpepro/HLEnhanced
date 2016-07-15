#ifndef GAME_SERVER_ANGELSCRIPT_CASGAMERULES_H
#define GAME_SERVER_ANGELSCRIPT_CASGAMERULES_H

#include <string>

#include <Angelscript/util/CASExtendAdapter.h>

#include "gamerules/CHalfLifeMultiplay.h"

class CASGameRules : public CASExtendAdapter<CHalfLifeMultiplay, CASGameRules>
{
public:
	typedef CASExtendAdapter<CHalfLifeMultiplay, CASGameRules> BaseClass;

public:
	using BaseClass::BaseClass;

	//TODO: RefreshSkillData

	void Think() override
	{
		CALL_EXTEND_FUNC_DIFFFUNC( Think, BaseClass::Think, "()" );
	}

	bool IsAllowedToSpawn( CBaseEntity* pEntity ) override
	{
		CALL_EXTEND_FUNC_RET( bool, IsAllowedToSpawn, "(CBaseEntity@)", pEntity );
	}

	bool FAllowFlashlight() const override
	{
		CALL_EXTEND_FUNC_RET( bool, FAllowFlashlight, "() const" );
	}

	bool IsMultiplayer() const override
	{
		CALL_EXTEND_FUNC_RET( bool, IsMultiplayer, "() const" );
	}

	bool IsDeathmatch() const override
	{
		CALL_EXTEND_FUNC_RET( bool, IsDeathmatch, "() const" );
	}

	bool IsTeamplay() const override
	{
		CALL_EXTEND_FUNC_RET( bool, IsTeamplay, "() const" );
	}

	bool IsCoOp() const override
	{
		CALL_EXTEND_FUNC_RET( bool, IsCoOp, "() const" );
	}

	const char* GetGameDescription() const override
	{
		if( auto pFunction = GetObject().GetTypeInfo()->GetMethodByDecl( "string GetGameDescription() const" ) )
		{
			CASOwningContext ctx( *pFunction->GetEngine() );
			
			CASMethod method( *pFunction, ctx, GetObject().Get() );

			static std::string result;
			
			if( method.Call( CallFlag::NONE ) )
			{
				method.GetReturnValue( &result );
			}

			return result.c_str();
		}
		else
		{
			return BaseClass::GetGameDescription();
		}
	}
};

#endif //GAME_SERVER_ANGELSCRIPT_CASGAMERULES_H