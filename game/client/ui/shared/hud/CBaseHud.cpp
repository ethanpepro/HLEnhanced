#include "hud.h"
#include "cl_util.h"

#include "CHudElement.h"

#include "CBaseHud.h"

CBaseHud::CBaseHud()
{
}

CBaseHud::~CBaseHud()
{
	//Thanks to Valve not making the engine call IClientVGUI::Shutdown for games other than CS & CZero, we can't clean up properly.
	//So instead we have to remove all elements only when the game hasn't shut down, because that means the instance is being destructed on dll unload.
	//We can't free panels anymore because the parents are gone, as is vgui2's library. - Solokiller
	//TODO: if Valve ever fixes this, destruct the Hud in CClientVGUI::Shutdown.
	if( !m_bGameShutdown )
	{
		RemoveAllElements();
	}
}

void CBaseHud::GameShutdown()
{
	m_bGameShutdown = true;
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
