#ifndef GAME_SERVER_ANGELSCRIPT_CHLASCLIENTINITIALIZER_H
#define GAME_SERVER_ANGELSCRIPT_CHLASCLIENTINITIALIZER_H

#include "Angelscript/CHLASBaseInitializer.h"

class CHLASClientInitializer final : public CHLASBaseInitializer
{
public:
	using CHLASBaseInitializer::CHLASBaseInitializer;

	bool RegisterCoreAPI( CASManager& manager ) override;
};

#endif //GAME_SERVER_ANGELSCRIPT_CHLASCLIENTINITIALIZER_H