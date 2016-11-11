//TODO
#define NO_CLAMP_DEF
#include "hud.h"
#include "cl_util.h"

#include "CHud2Element.h"

#include "CBaseHud2.h"

CBaseHud2::CBaseHud2()
{
}

CBaseHud2::~CBaseHud2()
{
	RemoveAllElements();
}

int CBaseHud2::GetElementCount() const
{
	return m_Elements.Count();
}

CHud2Element* CBaseHud2::GetElementByIndex( int iIndex )
{
	ASSERT( iIndex >= 0 && iIndex < m_Elements.Count() );

	if( iIndex < 0 || iIndex >= m_Elements.Count() )
		return nullptr;

	return m_Elements[ iIndex ];
}

CHud2Element* CBaseHud2::GetElementByName( const char* const pszName )
{
	ASSERT( pszName );

	auto count = m_Elements.Count();

	for( decltype( count ) iIndex = 0; iIndex < count; ++iIndex )
	{
		if( Q_strcmp( m_Elements[ iIndex ]->GetName(), pszName ) == 0 )
			return m_Elements[ iIndex ];
	}

	return nullptr;
}

bool CBaseHud2::AddElement( CHud2Element* pElement )
{
	if( !pElement )
		return false;

	if( GetElementByName( pElement->GetName() ) )
	{
		Con_Printf( "CBaseHud2::AddElement: Attempted to add element '%s' multiple times.\n", pElement->GetName() );
		return false;
	}

	m_Elements.AddToTail( pElement );

	return true;
}

void CBaseHud2::RemoveElement( CHud2Element* pElement, const bool bDelete )
{
	if( !pElement )
		return;

	auto count = m_Elements.Count();

	for( decltype( count ) iIndex = 0; iIndex < count; ++iIndex )
	{
		if( m_Elements[ iIndex ] == pElement )
		{
			m_Elements.Remove( iIndex );

			if( bDelete )
				delete pElement;

			return;
		}
	}
}

void CBaseHud2::RemoveAllElements( const bool bDelete )
{
	if( bDelete )
		m_Elements.PurgeAndDeleteElements();
	else
		m_Elements.Purge();
}
