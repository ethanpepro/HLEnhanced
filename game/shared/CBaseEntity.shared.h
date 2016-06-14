#ifndef GAME_SHARED_CBASEENTITY_SHARED_H
#define GAME_SHARED_CBASEENTITY_SHARED_H

//
// Base Entity.  All entity types derive from this
//
class CBaseEntity
{
public:
	DECLARE_CLASS_NOBASE( CBaseEntity );
	DECLARE_DATADESC_NOBASE();

	// Constructor.  Set engine to use C/C++ callback functions
	// pointers to engine data
	entvars_t *pev;		// Don't need to save/restore this pointer, the engine resets it

						// path corners
	CBaseEntity			*m_pGoalEnt;// path corner we are heading towards
	CBaseEntity			*m_pLink;// used for temporary link-list operations. 

	/**
	*	Called when the entity is first created. - Solokille
	*/
	virtual void OnCreate() {}

	/**
	*	Called when the entity is destroyed. - Solokiller
	*/
	virtual void  OnDestroy() {}

	// initialization functions
	virtual void	Spawn( void ) { return; }
	virtual void	Precache( void ) { return; }
	virtual void	KeyValue( KeyValueData* pkvd ) { pkvd->fHandled = false; }
	virtual bool	Save( CSave &save );
	virtual bool	Restore( CRestore &restore );
	virtual int		ObjectCaps( void ) { return FCAP_ACROSS_TRANSITION; }
	virtual void	Activate( void ) {}

	// Setup the object->object collision box (pev->mins / pev->maxs is the object->world collision box)
	virtual void	SetObjectCollisionBox( void );

	// Classify - returns the type of group (i.e, "houndeye", or "human military" so that monsters with different classnames
	// still realize that they are teammates. (overridden for monsters that form groups)
	virtual int Classify() { return CLASS_NONE; };
	virtual void DeathNotice( entvars_t *pevChild ) {}// monster maker children use this to tell the monster maker that they have died.

	virtual void	TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType );
	virtual int		TakeDamage( entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType );

	/**
	*	Gives health to this entity. Negative values take health.
	*	Used to be called TakeHealth.
	*	- Solokiller
	*	@param flHealth Amount of health to give. Negative values take health.
	*	@param bitsDamageType Damage types bit vector. @see Damage enum.
	*	@return Actual amount of health that was given/taken.
	*/
	virtual float	GiveHealth( float flHealth, int bitsDamageType );

	virtual void	Killed( entvars_t *pevAttacker, int iGib );
	virtual int		BloodColor( void ) { return DONT_BLEED; }
	virtual void	TraceBleed( float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType );
	virtual bool    IsTriggered( const CBaseEntity* const pActivator ) const { return true; }
	virtual CBaseMonster *MyMonsterPointer( void ) { return NULL; }
	virtual CSquadMonster *MySquadMonsterPointer( void ) { return NULL; }
	virtual	int		GetToggleState( void ) { return TS_AT_TOP; }

	virtual float	GetDelay( void ) { return 0; }
	virtual bool	IsMoving() const { return pev->velocity != g_vecZero; }
	virtual void	OverrideReset( void ) {}
	virtual int		DamageDecal( int bitsDamageType );
	// This is ONLY used by the node graph to test movement through a door
	virtual void	SetToggleState( int state ) {}
	virtual void    StartSneaking( void ) {}
	virtual void    StopSneaking( void ) {}
	virtual bool	OnControls( entvars_t *pev ) { return false; }
	virtual bool    IsSneaking() { return false; }
	virtual bool	IsAlive() const { return ( pev->deadflag == DEAD_NO ) && pev->health > 0; }
	virtual bool	IsBSPModel() const { return pev->solid == SOLID_BSP || pev->movetype == MOVETYPE_PUSHSTEP; }
	virtual bool	ReflectGauss() const { return ( IsBSPModel() && !pev->takedamage ); }
	virtual bool	HasTarget( string_t targetname ) const { return FStrEq( STRING( targetname ), STRING( pev->targetname ) ); }
	virtual bool    IsInWorld() const;
	virtual	bool	IsPlayer() const { return false; }
	virtual bool	IsNetClient() const { return false; }
	virtual const char *TeamID() const { return ""; }


	//	virtual void	SetActivator( CBaseEntity *pActivator ) {}
	virtual CBaseEntity *GetNextTarget( void );

	// fundamental callbacks
	void ( CBaseEntity ::*m_pfnThink )( void );
	void ( CBaseEntity ::*m_pfnTouch )( CBaseEntity *pOther );
	void ( CBaseEntity ::*m_pfnUse )( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void ( CBaseEntity ::*m_pfnBlocked )( CBaseEntity *pOther );

	virtual void Think( void ) { if( m_pfnThink ) ( this->*m_pfnThink )( ); };
	virtual void Touch( CBaseEntity *pOther ) { if( m_pfnTouch ) ( this->*m_pfnTouch )( pOther ); };
	virtual void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
	{
		if( m_pfnUse )
			( this->*m_pfnUse )( pActivator, pCaller, useType, value );
	}
	virtual void Blocked( CBaseEntity *pOther ) { if( m_pfnBlocked ) ( this->*m_pfnBlocked )( pOther ); };

	// allow engine to allocate instance data
	void *operator new( size_t stAllocateBlock, entvars_t *pev )
	{
		return ( void * ) ALLOC_PRIVATE( ENT( pev ), stAllocateBlock );
	};

	// don't use this.
#if _MSC_VER >= 1200 // only build this code if MSVC++ 6.0 or higher
	void operator delete( void *pMem, entvars_t *pev )
	{
		pev->flags |= FL_KILLME;
	};
#endif

	void UpdateOnRemove( void );

	// common member functions
	void EXPORT SUB_Remove( void );
	void EXPORT SUB_DoNothing( void );
	void EXPORT SUB_StartFadeOut( void );
	void EXPORT SUB_FadeOut( void );
	void EXPORT SUB_CallUseToggle( void ) { this->Use( this, this, USE_TOGGLE, 0 ); }
	bool ShouldToggle( USE_TYPE useType, const bool currentState ) const;
	void		FireBullets( ULONG	cShots, Vector  vecSrc, Vector	vecDirShooting, Vector	vecSpread, float flDistance, int iBulletType, int iTracerFreq = 4, int iDamage = 0, entvars_t *pevAttacker = NULL );
	Vector		FireBulletsPlayer( ULONG	cShots, Vector  vecSrc, Vector	vecDirShooting, Vector	vecSpread, float flDistance, int iBulletType, int iTracerFreq = 4, int iDamage = 0, entvars_t *pevAttacker = NULL, int shared_rand = 0 );

	virtual CBaseEntity *Respawn( void ) { return NULL; }

	void SUB_UseTargets( CBaseEntity *pActivator, USE_TYPE useType, float value );
	// Do the bounding boxes of these two intersect?
	int		Intersects( CBaseEntity *pOther );
	void	MakeDormant( void );
	int		IsDormant( void );
	//Made this virtual. Used to be non-virtual and redeclared in CBaseToggle - Solokiller
	virtual bool    IsLockedByMaster() const { return false; }

	static CBaseEntity *Instance( edict_t *pent )
	{
		if( !pent )
			pent = ENT( 0 );
		CBaseEntity *pEnt = ( CBaseEntity * ) GET_PRIVATE( pent );
		return pEnt;
	}

	static CBaseEntity *Instance( entvars_t *pev ) { return Instance( ENT( pev ) ); }
	static CBaseEntity *Instance( int eoffset ) { return Instance( ENT( eoffset ) ); }

	CBaseMonster *GetMonsterPointer( entvars_t *pevMonster )
	{
		CBaseEntity *pEntity = Instance( pevMonster );
		if( pEntity )
			return pEntity->MyMonsterPointer();
		return NULL;
	}
	CBaseMonster *GetMonsterPointer( edict_t *pentMonster )
	{
		CBaseEntity *pEntity = Instance( pentMonster );
		if( pEntity )
			return pEntity->MyMonsterPointer();
		return NULL;
	}


	// Ugly code to lookup all functions to make sure they are exported when set.
#ifdef _DEBUG
	void FunctionCheck( void *pFunction, char *name )
	{
		if( pFunction && !NAME_FOR_FUNCTION( ( uint32 ) pFunction ) )
			ALERT( at_error, "No EXPORT: %s:%s (%08lx)\n", STRING( pev->classname ), name, ( uint32 ) pFunction );
	}

	BASEPTR	ThinkSet( BASEPTR func, char *name )
	{
		m_pfnThink = func;
		FunctionCheck( ( void * )*( ( int * ) ( ( char * )this + ( offsetof( CBaseEntity, m_pfnThink ) ) ) ), name );
		return func;
	}
	ENTITYFUNCPTR TouchSet( ENTITYFUNCPTR func, char *name )
	{
		m_pfnTouch = func;
		FunctionCheck( ( void * )*( ( int * ) ( ( char * )this + ( offsetof( CBaseEntity, m_pfnTouch ) ) ) ), name );
		return func;
	}
	USEPTR	UseSet( USEPTR func, char *name )
	{
		m_pfnUse = func;
		FunctionCheck( ( void * )*( ( int * ) ( ( char * )this + ( offsetof( CBaseEntity, m_pfnUse ) ) ) ), name );
		return func;
	}
	ENTITYFUNCPTR	BlockedSet( ENTITYFUNCPTR func, char *name )
	{
		m_pfnBlocked = func;
		FunctionCheck( ( void * )*( ( int * ) ( ( char * )this + ( offsetof( CBaseEntity, m_pfnBlocked ) ) ) ), name );
		return func;
	}

#endif


	// virtual functions used by a few classes

	// used by monsters that are created by the MonsterMaker
	virtual	void UpdateOwner( void ) { return; };


	//
	static CBaseEntity *Create( char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner = NULL );

	virtual bool FBecomeProne() { return false; }
	edict_t *edict() { return ENT( pev ); }
	const edict_t* edict() const { return ENT( pev ); }
	EOFFSET eoffset() { return OFFSET( pev ); }
	int	  entindex() { return ENTINDEX( edict() ); }

	//TODO: make these const correct - Solokiller
	virtual Vector Center() { return ( pev->absmax + pev->absmin ) * 0.5; }; // center point of entity
	virtual Vector EyePosition() const { return pev->origin + pev->view_ofs; };			// position of eyes
	virtual Vector EarPosition() { return pev->origin + pev->view_ofs; };			// position of ears
	virtual Vector BodyTarget( const Vector &posSrc ) { return Center(); };		// position to shoot at

	virtual int Illumination() { return GETENTITYILLUM( ENT( pev ) ); };

	virtual	bool FVisible( const CBaseEntity *pEntity ) const;
	virtual	bool FVisible( const Vector &vecOrigin ) const;

	//We use this variables to store each ammo count.
	int ammo_9mm;
	int ammo_357;
	int ammo_bolts;
	int ammo_buckshot;
	int ammo_rockets;
	int ammo_uranium;
	int ammo_hornets;
	int ammo_argrens;
	//Special stuff for grenades and satchels.
	float m_flStartThrow;
	float m_flReleaseThrow;
	int m_chargeReady;
	int m_fInAttack;

	enum EGON_FIRESTATE { FIRE_OFF, FIRE_CHARGE };
	int m_fireState;
};

// Ugly technique to override base member functions
// Normally it's illegal to cast a pointer to a member function of a derived class to a pointer to a 
// member function of a base class.  static_cast is a sleezy way around that problem.

#ifdef _DEBUG

#define SetThink( a ) ThinkSet( static_cast <void (CBaseEntity::*)(void)> (a), #a )
#define SetTouch( a ) TouchSet( static_cast <void (CBaseEntity::*)(CBaseEntity *)> (a), #a )
#define SetUse( a ) UseSet( static_cast <void (CBaseEntity::*)(	CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )> (a), #a )
#define SetBlocked( a ) BlockedSet( static_cast <void (CBaseEntity::*)(CBaseEntity *)> (a), #a )

#else

#define SetThink( a ) m_pfnThink = static_cast <void (CBaseEntity::*)(void)> (a)
#define SetTouch( a ) m_pfnTouch = static_cast <void (CBaseEntity::*)(CBaseEntity *)> (a)
#define SetUse( a ) m_pfnUse = static_cast <void (CBaseEntity::*)( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )> (a)
#define SetBlocked( a ) m_pfnBlocked = static_cast <void (CBaseEntity::*)(CBaseEntity *)> (a)

#endif

#endif //GAME_SHARED_CBASEENTITY_SHARED_H