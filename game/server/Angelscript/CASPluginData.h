#ifndef GAME_SERVER_ANGELSCRIPT_CASPLUGINDATA_H
#define GAME_SERVER_ANGELSCRIPT_CASPLUGINDATA_H

#include <Angelscript/CASModule.h>

#include "ASPluginConstants.h"

class CASPluginData final
{
public:
	CASPluginData( const PluginLifetime lifetime );

	PluginLifetime GetLifetime() const { return m_Lifetime; }

	void SetLifetime( const PluginLifetime lifetime )
	{
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
