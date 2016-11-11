#include "CHud2ElementRegistry.h"

#include "CHud2Health.h"

REGISTER_HUD2ELEMENT( CHud2Health );

CHud2Health::CHud2Health( const char* const pszName )
	: CHud2Element( pszName )
{
}
