#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "entities/CBasePlayer.h"
#include "weapons.h"

void CBasePlayerItem::SetObjectCollisionBox( void )
{
}

bool CBasePlayerItem::AddToPlayer( CBasePlayer* pPlayer )
{
	return true;
}

void CBasePlayerItem::DestroyItem( void )
{
}

void CBasePlayerItem::DefaultTouch( CBaseEntity* pOther )
{
}

void CBasePlayerItem::FallThink( void )
{
}

void CBasePlayerItem::Materialize( void )
{
}

void CBasePlayerItem::AttemptToMaterialize( void )
{
}

CBaseEntity* CBasePlayerItem::Respawn( void )
{
	return NULL;
}

void CBasePlayerItem::FallInit( void )
{
}

void CBasePlayerItem::CheckRespawn( void )
{
}

void CBasePlayerItem::Holster( int skiplocal )
{
}

void CBasePlayerItem::Drop( void )
{
}

void CBasePlayerItem::Kill( void )
{
}

void CBasePlayerItem::AttachToPlayer( CBasePlayer* pPlayer )
{
}