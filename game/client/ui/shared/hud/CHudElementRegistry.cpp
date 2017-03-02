#include "hud.h"
#include "cl_util.h"

#include "CHudElement.h"
#include "CHudList.h"

#include "CHudElementRegistry.h"

CHudElementRegistryEntry::CHudElementRegistryEntry( CHudElementRegistry& registry, CreateFn createFn, const CHudDefaultableArgs& args )
	: m_CreateFn( createFn )
	, m_iDepth( clamp( args.m_iDepth, HUD_ELEMENT_DEPTH_MIN, HUD_ELEMENT_DEPTH_MAX ) )
{
	//No elements yet or deepest element yet, add to front.
	if( !registry.m_pHead || m_iDepth >= registry.m_pHead->m_iDepth )
	{
		m_pNext = registry.m_pHead;
		registry.m_pHead = this;
	}
	else
	{
		//Sort the element in the list. The list is sorted deepest to shallowest, so that the last element renders on top of all others.

		decltype( this ) pPrev = nullptr;
		auto pCurrent = registry.m_pHead;

		while( pCurrent && m_iDepth < pCurrent->m_iDepth )
		{
			pPrev = pCurrent;
			pCurrent = pCurrent->GetNext();
		}

		pPrev->m_pNext = this;
		m_pNext = pCurrent;
	}
}

CHudElementRegistry::CHudElementRegistry( const char* pszName )
	: m_pszName( pszName )
{
}

size_t CHudElementRegistry::CreateAllElements( CHudList& hudList )
{
	size_t uiCount = 0;

	for( auto pReg = GetHead(); pReg; pReg = pReg->GetNext() )
	{
		auto pFn = pReg->GetCreateFunction();

		if( auto pElement = pFn() )
		{
			//Shouldn't happen, but just in case.
			if( hudList.AddElement( pElement ) )
			{
				++uiCount;
			}
			else
			{
				delete pElement;
			}
		}
	}

	return uiCount;
}
