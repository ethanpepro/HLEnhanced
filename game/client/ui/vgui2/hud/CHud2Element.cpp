#include "hud.h"
#include "cl_util.h"

#include "CHud2Element.h"

CHud2Element::CHud2Element( const char* const pszName )
	: m_pszName( pszName )
{
	ASSERT( pszName );
}

CHud2Element::~CHud2Element()
{
}
