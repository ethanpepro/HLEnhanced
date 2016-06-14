#ifndef GAME_SHARED_DATAMAPPING_H
#define GAME_SHARED_DATAMAPPING_H

#include "eiface.h"

#define DECLARE_CLASS_NOBASE( thisClass )	\
typedef thisClass ThisClass

#define DECLARE_CLASS( thisClass, baseClass )	\
DECLARE_CLASS_NOBASE( thisClass );				\
typedef baseClass BaseClass

/**
*	Represents the datamap for an entity.
*/
struct DataMap_t final
{
	/**
	*	Name of the class this belongs to.
	*/
	const char* pszClassName;

	/**
	*	Pointer to parent map, or null if no parent exists.
	*/
	const DataMap_t* pParent;

	/**
	*	Pointer to this entity's type description array.
	*/
	const TYPEDESCRIPTION* pTypeDesc;

	/**
	*	Number of descriptors in the type description array.
	*/
	size_t uiNumDescriptors;
};

/**
*	Specialized for each entity class that implements a datamap.
*	@tparam Entity class.
*	@return Whether the map was initialized or not.
*/
template<typename T>
bool InitDataMap()
{
	return false;
}

#define DECLARE_DATADESC_NOBASE()				\
private:										\
	static DataMap_t m_DataMap;					\
												\
	template<typename T>						\
	friend bool InitDataMap();					\
												\
public:											\
	static const DataMap_t* GetBaseDataMap();	\
	static const DataMap_t* GetThisDataMap();	\
	virtual const DataMap_t* GetDataMap() const

#define DECLARE_DATADESC()					\
DECLARE_DATADESC_NOBASE()

#define __BEGIN_DATADESC( thisClass )				\
													\
DataMap_t thisClass::m_DataMap;						\
													\
const DataMap_t* thisClass::GetThisDataMap()		\
{													\
	return &m_DataMap;								\
}													\
													\
const DataMap_t* thisClass::GetDataMap() const		\
{													\
	return &m_DataMap;								\
}													\
													\
template<>											\
bool InitDataMap<thisClass>();						\
													\
namespace __##thisClass##__Init						\
{													\
	const bool bInit = InitDataMap<thisClass>();	\
}													\
													\
template<>											\
bool InitDataMap<thisClass>()						\
{													\
	typedef thisClass ThisClass;					\
													\
	const char* const pszClassName = #thisClass;	\
													\
	static TYPEDESCRIPTION typeDesc[] =				\
	{

#define BEGIN_DATADESC_NOBASE( thisClass )			\
const DataMap_t* thisClass::GetBaseDataMap()		\
{													\
	return nullptr;									\
}													\
													\
__BEGIN_DATADESC( thisClass )

#define BEGIN_DATADESC( thisClass )					\
const DataMap_t* thisClass::GetBaseDataMap()		\
{													\
	return thisClass::BaseClass::GetThisDataMap();	\
}													\
													\
__BEGIN_DATADESC( thisClass )

#define END_DATADESC()										\
		DEFINE_FIELD( ThisClass, pev, FIELD_EVARS )			\
	};														\
															\
	DataMap_t* pDataMap = &ThisClass::m_DataMap;			\
	pDataMap->pszClassName = pszClassName;					\
	pDataMap->pParent = ThisClass::GetBaseDataMap();		\
	pDataMap->pTypeDesc = typeDesc;							\
	pDataMap->uiNumDescriptors = ARRAYSIZE( typeDesc ) - 1;	\
															\
	return true;											\
}

#endif //GAME_SHARED_DATAMAPPING_H