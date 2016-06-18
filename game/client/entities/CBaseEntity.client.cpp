#include "extdll.h"
#include "util.h"
#include "cbase.h"

void CBaseEntity::UpdateOnRemove()
{
}

bool CBaseEntity::Save( CSave &save )
{
	return true;
}

bool CBaseEntity::Restore( CRestore &restore )
{
	return true;
}

void CBaseEntity::SetObjectCollisionBox( void )
{
}

CBaseEntity* CBaseEntity::Create( char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner )
{
	return NULL;
}

void CBaseEntity::TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType )
{
}

void CBaseEntity::TraceBleed( float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType )
{
}

int CBaseEntity::TakeDamage( CBaseEntity* pInflictor, CBaseEntity* pAttacker, float flDamage, int bitsDamageType )
{
	return 1;
}

/*
=====================
CBaseEntity :: Killed

If weapons code "kills" an entity, just set its effects to EF_NODRAW
=====================
*/
void CBaseEntity::Killed( entvars_t *pevAttacker, GibAction gibAction )
{
	pev->effects |= EF_NODRAW;
}

float CBaseEntity::GiveHealth( float flHealth, int bitsDamageType )
{
	return flHealth;
}

int	CBaseEntity::DamageDecal( int bitsDamageType ) const
{
	return -1;
}

bool CBaseEntity::IsInWorld() const
{
	return true;
}

CBaseEntity* CBaseEntity::GetNextTarget( void )
{
	return NULL;
}

void CBaseEntity::SUB_Remove( void )
{
}

bool CBaseEntity::ShouldToggle( USE_TYPE useType, const bool currentState ) const
{
	return false;
}

void CBaseEntity::FireBullets( const unsigned int cShots,
							   Vector vecSrc, Vector vecDirShooting, Vector vecSpread,
							   float flDistance, int iBulletType,
							   int iTracerFreq, int iDamage, entvars_t *pevAttacker )
{
}

bool CBaseEntity::Intersects( const CBaseEntity* const pOther ) const
{
	return false;
}

void CBaseEntity::MakeDormant( void )
{
}

bool CBaseEntity::IsDormant() const
{
	return false;
}

bool CBaseEntity::FVisible( const CBaseEntity *pEntity ) const
{
	return false;
}

bool CBaseEntity::FVisible( const Vector &vecOrigin ) const
{
	return false;
}