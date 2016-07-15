#ifndef GAME_SERVER_ANGELSCRIPT_CASGAMERULES_H
#define GAME_SERVER_ANGELSCRIPT_CASGAMERULES_H

#include "Angelscript/util/CASExtendAdapter.h"

#include "gamerules/CHalfLifeMultiplay.h"

class CASGameRules : public CASExtendAdapter<CHalfLifeMultiplay, CASGameRules>
{
public:
	typedef CASExtendAdapter<CHalfLifeMultiplay, CASGameRules> BaseClass;

public:
	using BaseClass::BaseClass;

	void Think() override
	{
		CALL_EXTEND_FUNC_DIFFFUNC( Think, BaseClass::Think, "" );
	}
};

#endif //GAME_SERVER_ANGELSCRIPT_CASGAMERULES_H