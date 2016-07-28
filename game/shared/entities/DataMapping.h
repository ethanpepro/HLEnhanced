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

#define __DECLARE_DATADESC_NOBASE()				\
private:										\
	static DataMap_t m_DataMap;					\
												\
	template<typename T>						\
	friend bool InitDataMap();					\
												\
public:											\
	static const DataMap_t* GetBaseDataMap();	\
	static const DataMap_t* GetThisDataMap()

/**
*	Data descriptor for classes that have no vtable.
*/
#define DECLARE_DATADESC_FINAL()				\
__DECLARE_DATADESC_NOBASE();					\
	const DataMap_t* GetDataMap() const

/**
*	Data descriptor for the root class, if it has a vtable.
*/
#define DECLARE_DATADESC_NOBASE()				\
__DECLARE_DATADESC_NOBASE();					\
	virtual const DataMap_t* GetDataMap() const

/**
*	Data descriptor for subclasses, if they have vtables.
*/
#define DECLARE_DATADESC()						\
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

/**
*	Begins the data descriptor for the base class.
*/
#define BEGIN_DATADESC_NOBASE( thisClass )			\
const DataMap_t* thisClass::GetBaseDataMap()		\
{													\
	return nullptr;									\
}													\
													\
__BEGIN_DATADESC( thisClass )

/**
*	Begins the data descriptor for subclasses.
*/
#define BEGIN_DATADESC( thisClass )					\
const DataMap_t* thisClass::GetBaseDataMap()		\
{													\
	return thisClass::BaseClass::GetThisDataMap();	\
}													\
													\
__BEGIN_DATADESC( thisClass )

/**
*	Ends the data descriptor.
*/
#define END_DATADESC()											\
		{ FIELD_CHARACTER, "Dummy", 0, 0, 0 }					\
	};															\
																\
	DataMap_t* pDataMap = &ThisClass::m_DataMap;				\
	pDataMap->pszClassName = pszClassName;						\
	pDataMap->pParent = ThisClass::GetBaseDataMap();			\
	pDataMap->pTypeDesc = typeDesc;								\
	pDataMap->uiNumDescriptors = ARRAYSIZE( typeDesc ) - 1;		\
																\
	return true;												\
}

#endif //GAME_SHARED_DATAMAPPING_H