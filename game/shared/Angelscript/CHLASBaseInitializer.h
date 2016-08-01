#ifndef GAME_SHARED_ANGELSCRIPT_CHLASBASEINITIALIZER_H
#define GAME_SHARED_ANGELSCRIPT_CHLASBASEINITIALIZER_H

#include <Angelscript/IASInitializer.h>

class CHLASManager;

class CHLASBaseInitializer : public IASInitializer
{
public:
	CHLASBaseInitializer( CHLASManager& manager )
		: m_Manager( manager )
	{
	}

	bool GetMessageCallback( asSFuncPtr& outFuncPtr, void*& pOutObj, asDWORD& outCallConv ) override;

	bool RegisterCoreAPI( CASManager& manager ) override;

	bool RegisterAPI( CASManager& manager ) override;

protected:
	CHLASManager& m_Manager;
};

#endif //GAME_SHARED_ANGELSCRIPT_CHLASBASEINITIALIZER_H