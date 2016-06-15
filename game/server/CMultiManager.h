#ifndef GAME_SERVER_CMULTIMANAGER_H
#define GAME_SERVER_CMULTIMANAGER_H

//**********************************************************
// The Multimanager Entity - when fired, will fire up to 16 targets 
// at specified times.
// FLAG:		THREAD (create clones when triggered)
// FLAG:		CLONE (this is a clone for a threaded execution)

#define MAX_MULTI_TARGETS	16 // maximum number of targets a single multi_manager entity may be assigned.

#define SF_MULTIMAN_CLONE		0x80000000
#define SF_MULTIMAN_THREAD		0x00000001

class CMultiManager : public CBaseToggle
{
public:
	DECLARE_CLASS( CMultiManager, CBaseToggle );
	DECLARE_DATADESC();

	void KeyValue( KeyValueData *pkvd ) override;
	void Spawn( void ) override;
	void EXPORT ManagerThink( void );
	void EXPORT ManagerUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

#if _DEBUG
	void EXPORT ManagerReport( void );
#endif

	bool		HasTarget( string_t targetname ) const override;

	int ObjectCaps( void ) override { return CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	int		m_cTargets;	// the total number of targets in this manager's fire list.
	int		m_index;	// Current target
	float	m_startTime;// Time we started firing
	int		m_iTargetName[ MAX_MULTI_TARGETS ];// list if indexes into global string array
	float	m_flTargetDelay[ MAX_MULTI_TARGETS ];// delay (in seconds) from time of manager fire to target fire
private:
	inline bool IsClone() const { return ( pev->spawnflags & SF_MULTIMAN_CLONE ) != 0; }
	inline bool ShouldClone() const
	{
		if( IsClone() )
			return false;

		return ( pev->spawnflags & SF_MULTIMAN_THREAD ) != 0;
	}

	CMultiManager *Clone( void );
};

#endif //GAME_SERVER_CMULTIMANAGER_H