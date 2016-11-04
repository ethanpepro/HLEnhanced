#ifndef GAME_SERVER_ANGELSCRIPT_CASPLUGINDATA_H
#define GAME_SERVER_ANGELSCRIPT_CASPLUGINDATA_H

#include <Angelscript/CASModule.h>

#include "ASPluginConstants.h"

/**
*	Contains data about plugins.
*/
class CASPluginData final : public IASModuleUserData
{
public:
	/**
	*	@param lifetime The minimum lifetime of the plugin. This dictates when plugins can be reloaded.
	*/
	CASPluginData( const PluginLifetime lifetime );

	void Release() const override final
	{
		delete this;
	}

	/**
	*	@return The lifetime of the plugin.
	*/
	PluginLifetime GetLifetime() const { return m_Lifetime; }

	/**
	*	Sets the lifetime of the plugin.
	*/
	void SetLifetime( const PluginLifetime lifetime )
	{
		m_Lifetime = lifetime;
	}

	/**
	*	Sets the minimum lifetime of the plugin. Used by plugins themselves to guarantee a certain lifetime.
	*/
	void SetMinimumLifetime( const PluginLifetime lifetime )
	{
		if( lifetime > m_Lifetime )
			m_Lifetime = lifetime;
	}

	const PluginFlags_t& GetFlags() const { return m_Flags; }

	PluginFlags_t& GetFlags() { return m_Flags; }

private:
	PluginLifetime m_Lifetime;
	PluginFlags_t m_Flags = PluginFlag::NONE;

private:
	CASPluginData( const CASPluginData& ) = delete;
	CASPluginData& operator=( const CASPluginData& ) = delete;
};

inline CASPluginData::CASPluginData( const PluginLifetime lifetime )
	: m_Lifetime( lifetime )
{
}

inline CASPluginData* CASModule_GetPluginData( CASModule* pModule )
{
	return reinterpret_cast<CASPluginData*>( pModule->GetUserData() );
}

#endif //GAME_SERVER_ANGELSCRIPT_CASPLUGINDATA_H
