#include "hud.h"
#include "cl_util.h"

#include "CHudElement.h"

#include "CBaseHud.h"

CBaseHud::CBaseHud()
{
}

CBaseHud::~CBaseHud()
{
	Assert( m_Elements.Size() == 0 );
}

void CBaseHud::Shutdown()
{
	RemoveAllElements();
}

int CBaseHud::GetElementCount() const
{
	return m_Elements.Count();
}

CHudElement* CBaseHud::GetElementByIndex( int iIndex )
{
	ASSERT( iIndex >= 0 && iIndex < m_Elements.Count() );

	if( iIndex < 0 || iIndex >= m_Elements.Count() )
		return nullptr;

	return m_Elements[ iIndex ];
}

CHudElement* CBaseHud::GetElementByName( const char* const pszName )
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

bool CBaseHud::AddElement( CHudElement* pElement )
{
	if( !pElement )
		return false;

	if( GetElementByName( pElement->GetName() ) )
	{
		Con_Printf( "CBaseHud::AddElement: Attempted to add element '%s' multiple times.\n", pElement->GetName() );
		return false;
	}

	m_Elements.AddToTail( pElement );

	return true;
}

void CBaseHud::RemoveElement( CHudElement* pElement, const bool bDelete )
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

void CBaseHud::RemoveAllElements( const bool bDelete )
{
	if( bDelete )
		m_Elements.PurgeAndDeleteElements();
	else
		m_Elements.Purge();
}
