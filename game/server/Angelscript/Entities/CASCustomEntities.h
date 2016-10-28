#ifndef GAME_SERVER_ANGELSCRIPT_ENTITIES_CASCUSTOMENTITIES_H
#define GAME_SERVER_ANGELSCRIPT_ENTITIES_CASCUSTOMENTITIES_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <angelscript.h>

#include <Angelscript/util/CASRefPtr.h>

struct entvars_t;
struct edict_t;

class CBaseEntity;

class CASClassWriter;

/**
*	Manages the list of custom entity base classes and registered classes for the current map.
*/
class CASCustomEntities final
{
public:
	using CreateFn = void ( * )( entvars_t* );

	struct BaseClassData_t final
	{
		BaseClassData_t( std::string&& szClassName, std::string&& szCPPClassName, std::string&& szBaseClassName, std::string&& szClassDeclaration, const CreateFn createFn )
			: szClassName( std::move( szClassName ) )
			, szCPPClassName( std::move( szCPPClassName ) )
			, szBaseClassName( std::move( szBaseClassName ) )
			, szClassDeclaration( std::move( szClassDeclaration ) )
			, createFn( createFn )
		{
		}

		const std::string szClassName;
		const std::string szCPPClassName;
		const std::string szBaseClassName;
		const std::string szClassDeclaration;
		const CreateFn createFn;
	};

	using BaseClassList_t = std::vector<BaseClassData_t>;

	/**
	*	Represents a custom entity class.
	*/
	class CCustomEntityClass final
	{
	public:
		/**
		*	@param szMapName Name of this entity. This is the name that the fgd refers to.
		*	@param typeInfo Type info that represents the class.
		*	@param baseClassData Base class data that represents this custom entity's base class.
		*/
		CCustomEntityClass( std::string&& szMapName, CASRefPtr<asITypeInfo>&& typeInfo, const BaseClassData_t& baseClassData )
			: m_szMapName( std::move( szMapName ) )
			, m_TypeInfo( std::move( typeInfo ) )
			, m_BaseClassData( baseClassData )
		{
		}

		const std::string& GetMapName() const { return m_szMapName; }

		const CASRefPtr<asITypeInfo>& GetTypeInfo() const { return m_TypeInfo; }

		const BaseClassData_t& GetBaseClassData() const { return m_BaseClassData; }

	private:
		std::string m_szMapName;
		CASRefPtr<asITypeInfo> m_TypeInfo;

		const BaseClassData_t& m_BaseClassData;
	};

	using ClassList_t = std::vector<std::unique_ptr<CCustomEntityClass>>;

public:
	CASCustomEntities() = default;
	~CASCustomEntities() = default;

	const BaseClassList_t& GetBaseClassList() const { return m_BaseClasses; }

	/**
	*	Initializes the custom entities system.
	*/
	bool Initialize();

	/**
	*	Shuts down the custom entities system.
	*/
	void Shutdown();

	void WorldCreated();

	void WorldActivated();

	void WorldEnded();

	/**
	*	Finds a base class by class name.
	*/
	const BaseClassData_t* FindBaseClassByClassName( const char* const pszClassName ) const;

	/**
	*	Finds a custom class by map name.
	*/
	const CCustomEntityClass* FindCustomClassByMapName( const char* const pszMapName ) const;

	/**
	*	Registers a custom entity. Registration is only allowed between MapInit and MapActivate.
	*	@param szMapName Map name of the entity. Maps will use this name to create instances.
	*	@param szClassName Angelscript class name to instantiate for this entity. Must inherit from a custom entity base class.
	*	@return true if registration succeeded, false otherwise.
	*/
	bool RegisterCustomEntity( const std::string& szMapName, const std::string& szClassName );

	/**
	*	Unregisters a custom entity class.
	*	@param szMapName Map name of the entity.
	*	@param szClassName Angelscript class name.
	*	@return true if the class was removed, false otherwise.
	*/
	bool UnregisterCustomEntity( const std::string& szMapName, const std::string& szClassName );

	/**
	*	Creates a custom entity by name. Used by scripts.
	*/
	CBaseEntity* CreateCustomEntity( const std::string& szMapName );

	/**
	*	Creates a custom entity by name. The given string must be either ALLOC_STRING'd or a static string.
	*/
	CBaseEntity* CreateCustomEntity( const char* const pszMapName );

	/**
	*	Creates a custom entity by name, and assigns it to the given edict. The given string must be either ALLOC_STRING'd or a static string.
	*/
	CBaseEntity* CreateCustomEntity( const char* const pszMapName, edict_t* pEdict );

	void OutputBaseClassesToFile( const char* const pszFileName ) const;

private:
	void GenerateBaseClasses();

	static bool CallInitMethod( const asITypeInfo& typeInfo, void* pInstance, CBaseEntity* pCPPInstance, const char* const pszMethod );

private:
	BaseClassList_t m_BaseClasses;
	ClassList_t m_ClassList;

	bool m_bAllowRegistration = false;

private:
	CASCustomEntities( const CASCustomEntities& ) = delete;
	CASCustomEntities& operator=( const CASCustomEntities& ) = delete;
};

extern CASCustomEntities g_CustomEntities;

#endif //GAME_SERVER_ANGELSCRIPT_ENTITIES_CASCUSTOMENTITIES_H
