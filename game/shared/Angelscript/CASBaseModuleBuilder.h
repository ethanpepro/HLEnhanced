#ifndef GAME_SHARED_ANGELSCRIPT_CASEBASEMODULEBUILDER_H
#define GAME_SHARED_ANGELSCRIPT_CASEBASEMODULEBUILDER_H

#include <string>
#include <vector>

#include <Angelscript/IASModuleBuilder.h>

/**
*	Base class for builders that handle internal and regular scripts.
*	Internal scripts are scripts that the application itself defines. These usually contain base classes for extension classes.
*	Regular scripts are loaded from disk.
*/
class CASBaseModuleBuilder : public IASModuleBuilder
{
public:
	typedef std::vector<std::pair<std::string, std::string>> InternalScripts_t;
	typedef std::vector<std::string> Scripts_t;

public:
	/**
	*	Constructor.
	*	@param szBasePath Path to prepend to scripts when loading them.
	*	@param szModuleTypeName Name of the module type being built.
	*/
	CASBaseModuleBuilder( std::string&& szBasePath, std::string&& szModuleTypeName );
	~CASBaseModuleBuilder() = default;

	/**
	*	@return The base path.
	*/
	const std::string& GetBasePath() const { return m_szBasePath; }

	/**
	*	@return List of internal scripts.
	*/
	const InternalScripts_t& GetInternalScripts() const { return m_InternalScripts; }

	/**
	*	@return List of scripts.
	*/
	const Scripts_t& GetScripts() const { return m_Scripts; }

	/**
	*	@param pszName Script name.
	*	@return Whether the given internal script is in the list.
	*/
	bool HasInternalScript( const char* const pszName ) const;

	/**
	*	@param szName Script name.
	*	@param szContents Script contents.
	*	@return Whether the given internal script has been added.
	*/
	bool AddInternalScript( std::string&& szName, std::string&& szContents );

	/**
	*	@param pszName Script name.
	*	@return Whether the given script is in the list.
	*/
	bool HasScript( const char* const pszName ) const;

	/**
	*	@param szName Script name.
	*	@return Whether the given script has been added.
	*/
	bool AddScript( std::string&& szName );

	bool DefineWords( CScriptBuilder& builder ) override;

	bool AddScripts( CScriptBuilder& builder ) override;

	bool PreBuild( CScriptBuilder& builder ) override;

	bool PostBuild( CScriptBuilder& builder, const bool bSuccess, CASModule* pModule ) override;

private:
	std::string m_szBasePath;
	std::string m_szModuleTypeName;
	InternalScripts_t m_InternalScripts;
	std::vector<std::string> m_Scripts;
};

#endif //GAME_SHARED_ANGELSCRIPT_CASEBASEMODULEBUILDER_H