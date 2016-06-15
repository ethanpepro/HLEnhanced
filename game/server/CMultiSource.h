#ifndef GAME_SERVER_CMULTISOURCE_H
#define GAME_SERVER_CMULTISOURCE_H

//
// MultiSouce
//

#define MS_MAX_TARGETS 32

class CMultiSource : public CPointEntity
{
public:
	DECLARE_CLASS( CMultiSource, CPointEntity );
	DECLARE_DATADESC();

	void Spawn() override;
	void KeyValue( KeyValueData *pkvd ) override;
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) override;
	int	ObjectCaps( void ) override { return ( CPointEntity::ObjectCaps() | FCAP_MASTER ); }
	bool IsTriggered( const CBaseEntity* const pActivator ) const override;
	void EXPORT Register( void );

	EHANDLE		m_rgEntities[ MS_MAX_TARGETS ];
	int			m_rgTriggered[ MS_MAX_TARGETS ];

	int			m_iTotal;
	string_t	m_globalstate;
};

#endif //GAME_SERVER_CMULTISOURCE_H