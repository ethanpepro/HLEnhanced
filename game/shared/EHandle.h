#ifndef GAME_SHARED_EHANDLE_H
#define GAME_SHARED_EHANDLE_H

typedef struct edict_s edict_t;

class CBaseEntity;

//
// EHANDLE. Safe way to point to CBaseEntities who may die between frames
//
class EHANDLE
{
private:
	edict_t *m_pent;
	int		m_serialnumber;

public:
	EHANDLE( CBaseEntity* pEntity = nullptr );
	EHANDLE( const EHANDLE& other );
	EHANDLE( edict_t* pEdict, int iSerialNumber );

	edict_t *Get() const;
	edict_t *Set( edict_t *pent );

	operator CBaseEntity *( );

	operator const CBaseEntity*( ) const;

	CBaseEntity * operator = ( CBaseEntity *pEntity );
	CBaseEntity * operator ->() const;

	const CBaseEntity* GetEntity() const { return *this; }
	CBaseEntity* GetEntity() { return *this; }
};

#endif //GAME_SHARED_EHANDLE_H