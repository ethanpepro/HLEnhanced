#if USE_OPFOR
/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "Weapons.h"
#include "CBasePlayer.h"

#ifndef CLIENT_DLL
#include "entities/rope/CRope.h"
#include "entities/spawnpoints/CBaseSpawnPoint.h"
#include "entities/weapons/CDisplacerBall.h"

#include "gamerules/GameRules.h"
#endif

#include "CDisplacer.h"

extern CBaseEntity* g_pLastSpawn;

#ifndef CLIENT_DLL
LINK_ENTITY_TO_CLASS( info_displacer_xen_target, CPointEntity );
#endif

LINK_ENTITY_TO_CLASS( weapon_displacer, CDisplacer );

CDisplacer::CDisplacer()
	: BaseClass( WEAPON_DISPLACER )
{
}

void CDisplacer::Precache()
{
	BaseClass::Precache();

	PRECACHE_MODEL( "models/v_displacer.mdl" );
	PRECACHE_MODEL( "models/w_displacer.mdl" );
	PRECACHE_MODEL( "models/p_displacer.mdl" );

	PRECACHE_SOUND( "weapons/displacer_fire.wav" );
	PRECACHE_SOUND( "weapons/displacer_self.wav" );
	PRECACHE_SOUND( "weapons/displacer_spin.wav" );
	PRECACHE_SOUND( "weapons/displacer_spin2.wav" );

	PRECACHE_SOUND( "buttons/button11.wav" );

	m_iSpriteTexture = PRECACHE_MODEL( "sprites/shockwave.spr" );

	UTIL_PrecacheOther( "displacer_ball" );
}

void CDisplacer::Spawn()
{
	Precache();

	SetModel( "models/w_displacer.mdl" );

	FallInit();
}

bool CDisplacer::AddToPlayer( CBasePlayer* pPlayer )
{
	if( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return true;
	}
	return false;
}

bool CDisplacer::Deploy()
{
	return DefaultDeploy( 
		"models/v_displacer.mdl", "models/p_displacer.mdl", 
		DISPLACER_DRAW, "egon" );
}

void CDisplacer::Holster()
{
	m_fInReload = false;

	for( auto& pBeam : m_pNoseBeam )
	{
		if( pBeam )
		{
			UTIL_Remove( pBeam );
			pBeam = nullptr;
		}
	}

	EMIT_SOUND_DYN( 
		m_pPlayer, 
		CHAN_WEAPON, "weapons/displacer_spin.wav", 
		0, ATTN_NONE, SND_STOP, PITCH_NORM );

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0;

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0.0, 5.0 );

	SendWeaponAnim( DISPLACER_HOLSTER1 );

	if( m_pfnThink == &CDisplacer::SpinupThink )
	{
		m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] -= 20;
		SetThink( nullptr );
	}
	else if( m_pfnThink == &CDisplacer::AltSpinupThink )
	{
		m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] -= 60;
		SetThink( nullptr );
	}
}

void CDisplacer::WeaponIdle()
{
	ResetEmptySound();

	if( m_flSoundDelay != 0 && gpGlobals->time >= m_flSoundDelay )
		m_flSoundDelay = 0;

	if( m_flTimeWeaponIdle <= UTIL_WeaponTimeBase() )
	{
		const float flNextIdle = UTIL_RandomFloat( 0, 1 );

		int iAnim;

		if( flNextIdle <= 0.5 )
		{
			iAnim = DISPLACER_IDLE1;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3.0;
		}
		else
		{
			iAnim = DISPLACER_IDLE2;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3.0;
		}

		SendWeaponAnim( iAnim );
	}
}

void CDisplacer::PrimaryAttack()
{
	if( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] >= 20 )
	{
		SetThink( &CDisplacer::SpinupThink );

		SetNextThink( gpGlobals->time );

		m_Mode = Mode::STARTED;

		EMIT_SOUND_DYN( 
			m_pPlayer, 
			CHAN_WEAPON, "weapons/displacer_spin.wav", 
			UTIL_RandomFloat( 0.8, 0.9 ), ATTN_NORM, 0, PITCH_NORM );

		m_flTimeWeaponIdle = m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 2.5;
	}
	else
	{
		EMIT_SOUND_DYN(
			m_pPlayer,
			CHAN_WEAPON, "buttons/button11.wav",
			UTIL_RandomFloat( 0.8, 0.9 ), ATTN_NORM, 0, PITCH_NORM );

		m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.5;
	}
}

void CDisplacer::SecondaryAttack()
{
	if( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] >= 60 )
	{
		SetThink( &CDisplacer::AltSpinupThink );

		SetNextThink( gpGlobals->time );

		m_Mode = Mode::STARTED;

		EMIT_SOUND_DYN(
			m_pPlayer,
			CHAN_WEAPON, "weapons/displacer_spin2.wav",
			UTIL_RandomFloat( 0.8, 0.9 ), ATTN_NORM, 0, PITCH_NORM );

		m_flTimeWeaponIdle = m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.5;
	}
	else
	{
		EMIT_SOUND_DYN(
			m_pPlayer,
			CHAN_WEAPON, "buttons/button11.wav",
			UTIL_RandomFloat( 0.8, 0.9 ), ATTN_NORM, 0, PITCH_NORM );

		m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.5;
	}
}

void CDisplacer::Reload()
{
	//Nothing
}

void CDisplacer::SpinupThink()
{
	if( m_Mode == Mode::STARTED )
	{
		SendWeaponAnim( DISPLACER_SPINUP );

		m_Mode = Mode::SPINNING_UP;

		CreateChargeEffect();

		m_flStartTime = gpGlobals->time;
		m_iSoundState = 0;
	}

	if( m_Mode <= Mode::SPINNING_UP )
	{
		if( gpGlobals->time > m_flStartTime + 0.9 )
		{
			m_Mode = Mode::SPINNING;

			SetThink( &CDisplacer::FireThink );

			SetNextThink( gpGlobals->time + 0.1 );
		}

		m_iImplodeCounter = static_cast<int>( floorf( ( gpGlobals->time - m_flStartTime ) * 100.0 + 50.0 ) );
	}

	if( m_iImplodeCounter > 250 )
		m_iImplodeCounter = 250;

	m_iSoundState = 128;

	SetNextThink( gpGlobals->time + 0.1 );
}

void CDisplacer::AltSpinupThink()
{
	if( m_Mode == Mode::STARTED )
	{
		SendWeaponAnim( DISPLACER_SPINUP );

		m_Mode = Mode::SPINNING_UP;

		CreateChargeEffect();

		m_flStartTime = gpGlobals->time;
		m_iSoundState = 0;
	}

	if( m_Mode <= Mode::SPINNING_UP )
	{
		if( gpGlobals->time > m_flStartTime + 0.9 )
		{
			m_Mode = Mode::SPINNING;

			SetThink( &CDisplacer::AltFireThink );

			SetNextThink( gpGlobals->time + 0.1 );
		}

		m_iImplodeCounter = static_cast<int>( floorf( ( gpGlobals->time - m_flStartTime ) * 100.0 + 50.0 ) );
	}

	if( m_iImplodeCounter > 250 )
		m_iImplodeCounter = 250;

	m_iSoundState = 128;

	SetNextThink( gpGlobals->time + 0.1 );
}

void CDisplacer::FireThink()
{
	m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] -= 20;

	DestroyChargeEffect();

	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	m_pPlayer->AddEffectsFlags( EF_MUZZLEFLASH );

	EMIT_SOUND_DYN( 
		m_pPlayer, 
		CHAN_WEAPON, "weapons/displacer_fire.wav", 
		UTIL_RandomFloat( 0.8, 0.9 ), ATTN_NORM, 0, PITCH_NORM );

#ifndef CLIENT_DLL
	const Vector vecAnglesAim = m_pPlayer->GetViewAngle() + m_pPlayer->GetPunchAngle();

	UTIL_MakeVectors( vecAnglesAim );

	Vector vecSrc = m_pPlayer->GetGunPosition();

	Vector vecAngles = m_pPlayer->GetAutoaimVectorFromPoint( vecSrc, AUTOAIM_10DEGREES );

	CDisplacerBall::CreateDisplacerBall( vecSrc, vecAnglesAim + vecAngles, m_pPlayer );

	if( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] == 0 )
	{
		m_pPlayer->SetSuitUpdate( "!HEV_AMO0", SUIT_SENTENCE,SUIT_REPEAT_OK );
	}

	Vector vecPunchAngle = m_pPlayer->GetPunchAngle();

	vecPunchAngle.x -= 2;

	m_pPlayer->SetPunchAngle( vecPunchAngle );
#endif

	SetThink( nullptr );
}

void CDisplacer::AltFireThink()
{
	DestroyChargeEffect();

#ifndef CLIENT_DLL
	if( m_pPlayer->IsOnRope() )
	{
		m_pPlayer->SetMoveType( MOVETYPE_WALK );
		m_pPlayer->SetSolidType( SOLID_SLIDEBOX );
		m_pPlayer->SetOnRopeState( false );
		m_pPlayer->GetRope()->DetachObject();
		m_pPlayer->SetRope( nullptr );
	}
#endif

	STOP_SOUND( m_pPlayer, CHAN_WEAPON, "weapons/displacer_spin.wav" );

	m_pPlayer->SetDisplacerReturn( m_pPlayer->GetAbsOrigin() );

	m_pPlayer->SetDisplacerSndRoomtype( m_pPlayer->m_flSndRoomtype );

	//TODO: CTF support - Solokiller
#if 0
	if( g_pGameRules->IsCTF() && m_pPlayer->m_hFlag )
	{
		CCTFFlag* pFlag = EHANDLE_cast<CCTFFlag*>( m_pPlayer->m_hFlag );

		pFlag->DropFlag( m_pPlayer );
	}
#endif

	CBaseEntity* pDestination;

#ifndef CLIENT_DLL
	if( !g_pGameRules->IsMultiplayer() || g_pGameRules->IsCoOp() )
	{
		pDestination = UTIL_FindEntityByClassname( nullptr, "info_displacer_xen_target" );
	}
	else
	{
		pDestination = g_pGameRules->GetPlayerSpawnSpot( m_pPlayer );

		if( !pDestination )
			pDestination = g_pLastSpawn;

		Vector vecEnd = pDestination->GetAbsOrigin();

		vecEnd.z -= 100;

		TraceResult tr;

		UTIL_TraceLine( pDestination->GetAbsOrigin(), vecEnd, ignore_monsters, edict(), &tr );

		m_pPlayer->SetAbsOrigin( tr.vecEndPos + Vector( 0, 0, 37 ) );
	}

	if( pDestination && pDestination != CWorld::GetInstance() )
	{
		m_pPlayer->ClearFlags( FL_SKIPLOCALHOST );

		Vector vecNewOrigin = pDestination->GetAbsOrigin();

		if( !g_pGameRules->IsMultiplayer() || g_pGameRules->IsCoOp() )
		{
			vecNewOrigin.z += 37;
		}

		m_pPlayer->SetAbsOrigin( vecNewOrigin );

		m_pPlayer->SetAbsAngles( pDestination->GetAbsAngles() );

		m_pPlayer->SetViewAngle( pDestination->GetAbsAngles() );

		m_pPlayer->SetFixAngleMode( FIXANGLE_SET );

		m_pPlayer->SetBaseVelocity( g_vecZero );
		m_pPlayer->SetAbsVelocity( g_vecZero );
#endif

		m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 2;

		SetThink( nullptr );

		m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

		EMIT_SOUND_DYN( 
			m_pPlayer, 
			CHAN_WEAPON, "weapons/displacer_self.wav", 
			UTIL_RandomFloat( 0.8, 0.9 ), ATTN_NORM, 0, PITCH_NORM );

		m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] -= 60;

#ifndef CLIENT_DLL
		CDisplacerBall::CreateDisplacerBall( m_pPlayer->GetDisplacerReturn(), Vector( 90, 0, 0 ), m_pPlayer );

		if( !m_iClip )
		{
			if( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] <= 0 )
				m_pPlayer->SetSuitUpdate( "!HEV_AMO0", SUIT_SENTENCE, SUIT_REPEAT_OK );
		}

		m_flTimeWeaponIdle = UTIL_WeaponTimeBase();

		if( !bIsMultiplayer() )
			m_pPlayer->SetGravity( 0.6 );
	}
	else
	{
		EMIT_SOUND_DYN(
			m_pPlayer,
			CHAN_WEAPON, "buttons/button11.wav",
			UTIL_RandomFloat( 0.8, 0.9 ), ATTN_NORM, 0, PITCH_NORM );
	}
#endif
}

void CDisplacer::CreateChargeEffect()
{
#ifndef CLIENT_DLL
	if( bIsMultiplayer() )
		return;

	const int iEntIndex = m_pPlayer->entindex();

	int iAttach = 0;

	for( auto& pBeam : m_pNoseBeam )
	{
		pBeam = CBeam::BeamCreate( "sprites/lgtning.spr", 16 );

		pBeam->EntsInit( iEntIndex, iEntIndex );

		pBeam->SetNoise( 60 );

		pBeam->SetWidth( 10 );

		pBeam->SetRenderColor( Vector( 96, 128, 16 ) );

		pBeam->SetScrollRate( 30 );

		pBeam->SetBrightness( 190 );

		//Last beam isn't attached to anything.
		if( iAttach <= 2 )
		{
			pBeam->SetStartAttachment( iAttach++ + 2 );

			pBeam->SetEndAttachment( iAttach % 2 + 2 );
		}
	}
#endif
}

void CDisplacer::DestroyChargeEffect()
{
	for( auto& pBeam : m_pNoseBeam )
	{
		if( pBeam )
		{
			UTIL_Remove( pBeam );
			pBeam = nullptr;
		}
	}
}
#endif //USE_OPFOR
