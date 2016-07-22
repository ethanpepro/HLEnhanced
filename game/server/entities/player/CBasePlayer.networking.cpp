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
#include "CBasePlayer.h"
#include "Weapons.h"

#include "pm_shared.h"

#include "gamerules/GameRules.h"

extern DLL_GLOBAL bool gDisplayTitle;

/*
=========================================================
UpdateClientData

resends any changed player HUD info to the client.
Called every frame by PlayerPreThink
Also called at start of demo recording and playback by
ForceClientDllUpdate to ensure the demo gets messages
reflecting all of the HUD state info.
=========================================================
*/
void CBasePlayer::UpdateClientData()
{
	//The engine will not call ClientPutInServer after transitions, so we'll have to catch this event every map change. - Solokiller
	if( !m_bSentInitData )
	{
		m_bSentInitData = true;

		//Send ammo types now. - Solokiller
		g_AmmoTypes.SendAmmoTypes( this );
	}

	if( m_fInitHUD )
	{
		m_fInitHUD = false;
		gInitHUD = false;

		MESSAGE_BEGIN( MSG_ONE, gmsgResetHUD, NULL, pev );
			WRITE_BYTE( 0 );
		MESSAGE_END();

		if( !m_fGameHUDInitialized )
		{
			MESSAGE_BEGIN( MSG_ONE, gmsgInitHUD, NULL, pev );
			MESSAGE_END();

			g_pGameRules->InitHUD( this );
			m_fGameHUDInitialized = true;

			m_iObserverLastMode = OBS_ROAMING;

			if( g_pGameRules->IsMultiplayer() )
			{
				FireTargets( "game_playerjoin", this, this, USE_TOGGLE, 0 );
			}
		}

		FireTargets( "game_playerspawn", this, this, USE_TOGGLE, 0 );

		InitStatusBar();
	}

	if( m_iHideHUD != m_iClientHideHUD )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgHideWeapon, NULL, pev );
			WRITE_BYTE( m_iHideHUD );
		MESSAGE_END();

		m_iClientHideHUD = m_iHideHUD;
	}

	if( m_iFOV != m_iClientFOV )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgSetFOV, NULL, pev );
			WRITE_BYTE( m_iFOV );
		MESSAGE_END();

		// cache FOV change at end of function, so weapon updates can see that FOV has changed
	}

	// HACKHACK -- send the message to display the game title
	if( gDisplayTitle )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgShowGameTitle, NULL, pev );
			WRITE_BYTE( 0 );
		MESSAGE_END();
		gDisplayTitle = false;
	}

	if( pev->health != m_iClientHealth )
	{
		int iHealth = clamp( static_cast<int>( pev->health ), 0, 255 );  // make sure that no negative health values are sent
		if( pev->health > 0.0f && pev->health <= 1.0f )
			iHealth = 1;

		// send "health" update message
		MESSAGE_BEGIN( MSG_ONE, gmsgHealth, NULL, pev );
			WRITE_BYTE( iHealth );
		MESSAGE_END();

		m_iClientHealth = pev->health;
	}


	if( pev->armorvalue != m_iClientBattery )
	{
		m_iClientBattery = pev->armorvalue;

		ASSERT( gmsgBattery > 0 );
		// send "health" update message
		MESSAGE_BEGIN( MSG_ONE, gmsgBattery, NULL, pev );
			WRITE_SHORT( ( int ) pev->armorvalue );
		MESSAGE_END();
	}

	if( pev->dmg_take || pev->dmg_save || m_bitsHUDDamage != m_bitsDamageType )
	{
		// Comes from inside me if not set
		Vector damageOrigin = GetAbsOrigin();
		// send "damage" message
		// causes screen to flash, and pain compass to show direction of damage
		edict_t *other = pev->dmg_inflictor;
		if( other )
		{
			CBaseEntity *pEntity = CBaseEntity::Instance( other );
			if( pEntity )
				damageOrigin = pEntity->Center();
		}

		// only send down damage type that have hud art
		int visibleDamageBits = m_bitsDamageType & DMG_SHOWNHUD;

		MESSAGE_BEGIN( MSG_ONE, gmsgDamage, NULL, pev );
			WRITE_BYTE( pev->dmg_save );
			WRITE_BYTE( pev->dmg_take );
			WRITE_LONG( visibleDamageBits );
			WRITE_COORD( damageOrigin.x );
			WRITE_COORD( damageOrigin.y );
			WRITE_COORD( damageOrigin.z );
		MESSAGE_END();

		//TODO: both of these can probably be removed. - Solokiller
		pev->dmg_take = 0;
		pev->dmg_save = 0;
		m_bitsHUDDamage = m_bitsDamageType;

		// Clear off non-time-based damage indicators
		m_bitsDamageType &= DMG_TIMEBASED;
	}

	// Update Flashlight
	if( ( m_flFlashLightTime ) && ( m_flFlashLightTime <= gpGlobals->time ) )
	{
		if( FlashlightIsOn() )
		{
			if( m_iFlashBattery )
			{
				m_flFlashLightTime = FLASH_DRAIN_TIME + gpGlobals->time;
				m_iFlashBattery--;

				if( !m_iFlashBattery )
					FlashlightTurnOff();
			}
		}
		else
		{
			if( m_iFlashBattery < 100 )
			{
				m_flFlashLightTime = FLASH_CHARGE_TIME + gpGlobals->time;
				m_iFlashBattery++;
			}
			else
				m_flFlashLightTime = 0;
		}

		MESSAGE_BEGIN( MSG_ONE, gmsgFlashBattery, NULL, pev );
			WRITE_BYTE( m_iFlashBattery );
		MESSAGE_END();
	}

	if( m_iTrain & TRAIN_NEW )
	{
		ASSERT( gmsgTrain > 0 );
		// send "health" update message
		MESSAGE_BEGIN( MSG_ONE, gmsgTrain, NULL, pev );
			WRITE_BYTE( m_iTrain & 0xF );
		MESSAGE_END();

		m_iTrain &= ~TRAIN_NEW;
	}

	//
	// New Weapon?
	//
	if( !m_bKnownItem )
	{
		m_bKnownItem = true;

		// WeaponInit Message
		// byte  = # of weapons
		//
		// for each weapon:
		// byte		name str length (not including null)
		// bytes... name
		// byte		Ammo Type
		// byte		Ammo2 Type
		// byte		bucket
		// byte		bucket pos
		// byte		flags	
		// ????		Icons

		// Send ALL the weapon info now
		for( int i = 0; i < MAX_WEAPONS; ++i )
		{
			ItemInfo& II = CBasePlayerItem::ItemInfoArray[ i ];

			if( !II.iId )
				continue;

			const char *pszName;
			if( !II.pszName )
				pszName = "Empty";
			else
				pszName = II.pszName;

			MESSAGE_BEGIN( MSG_ONE, gmsgWeaponList, NULL, pev );
			WRITE_STRING( pszName );			// string	weapon name
				WRITE_BYTE( GetAmmoIndex( II.pszAmmo1 ) );	// byte		Ammo Type
				WRITE_BYTE( GetAmmoIndex( II.pszAmmo2 ) );	// byte		Ammo2 Type
				WRITE_BYTE( II.iSlot );					// byte		bucket
				WRITE_BYTE( II.iPosition );				// byte		bucket pos
				WRITE_BYTE( II.iId );						// byte		id (bit index into pev->weapons)
				WRITE_BYTE( II.iFlags );					// byte		Flags
			MESSAGE_END();
		}
	}

	SendAmmoUpdate();

	// Update all the items
	for( int i = 0; i < MAX_WEAPON_SLOTS; i++ )
	{
		if( m_rgpPlayerItems[ i ] )  // each item updates it's successors
			m_rgpPlayerItems[ i ]->UpdateClientData( this );
	}

	// Cache and client weapon change
	m_pClientActiveItem = m_pActiveItem;
	m_iClientFOV = m_iFOV;

	// Update Status Bar
	if( m_flNextSBarUpdateTime < gpGlobals->time )
	{
		UpdateStatusBar();
		m_flNextSBarUpdateTime = gpGlobals->time + 0.2;
	}
}

/*
===============
ForceClientDllUpdate

When recording a demo, we need to have the server tell us the entire client state
so that the client side .dll can behave correctly.
Reset stuff so that the state is transmitted.
===============
*/
void CBasePlayer::ForceClientDllUpdate()
{
	m_iClientHealth = -1;
	m_iClientBattery = -1;
	m_iTrain |= TRAIN_NEW;	// Force new train message.
	m_fWeapon = false;		// Force weapon send
	m_bKnownItem = false;	// Force weaponinit messages.
	m_fInitHUD = true;		// Force HUD gmsgResetHUD message

	// Now force all the necessary messages to be sent.
	UpdateClientData();
}