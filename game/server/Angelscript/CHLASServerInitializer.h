#ifndef GAME_SERVER_ANGELSCRIPT_CHLASSERVERINITIALIZER_H
#define GAME_SERVER_ANGELSCRIPT_CHLASSERVERINITIALIZER_H

#include "Angelscript/CHLASBaseInitializer.h"

class CHLASServerInitializer final : public CHLASBaseInitializer
{
public:
	using CHLASBaseInitializer::CHLASBaseInitializer;

	bool RegisterCoreAPI( CASManager& manager ) override;

	bool AddEvents( CASManager& manager, CASEventManager& eventManager ) override;
};

#endif //GAME_SERVER_ANGELSCRIPT_CHLASSERVERINITIALIZER_H