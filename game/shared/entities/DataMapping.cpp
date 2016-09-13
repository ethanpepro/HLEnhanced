#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "DataMapping.h"

const TYPEDESCRIPTION* UTIL_FindTypeDescInSingleDataMap( const DataMap_t& dataMap, const char* const pszFieldName, const bool bComparePublicName )
{
	ASSERT( pszFieldName );

	const TYPEDESCRIPTION* pDesc;

	const char* pszName;

	for( size_t uiIndex = 0; uiIndex < dataMap.uiNumDescriptors; ++uiIndex )
	{
		pDesc = &dataMap.pTypeDesc[ uiIndex ];

		pszName = bComparePublicName ? pDesc->pszPublicName : pDesc->fieldName;

		if( pszName && stricmp( pszName, pszFieldName ) == 0 )
			return pDesc;
	}

	return nullptr;
}

const TYPEDESCRIPTION* UTIL_FindTypeDescInDataMap( const DataMap_t& dataMap, const char* const pszFieldName, const bool bComparePublicName )
{
	const DataMap_t* pMap = &dataMap;

	while( pMap )
	{
		if( auto pDesc = UTIL_FindTypeDescInSingleDataMap( *pMap, pszFieldName, bComparePublicName ) )
			return pDesc;

		pMap = pMap->pParent;
	}

	return nullptr;
}
