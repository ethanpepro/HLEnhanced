#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "weapons.h"
#include "player.h"

// Global Savedata for Delay
BEGIN_DATADESC_NOBASE( CBaseEntity )
	DEFINE_FIELD( CBaseEntity, m_pGoalEnt, FIELD_CLASSPTR ),
	
	DEFINE_FIELD( CBaseEntity, m_pfnThink, FIELD_FUNCTION ),		// UNDONE: Build table of these!!!
	DEFINE_FIELD( CBaseEntity, m_pfnTouch, FIELD_FUNCTION ),
	DEFINE_FIELD( CBaseEntity, m_pfnUse, FIELD_FUNCTION ),
	DEFINE_FIELD( CBaseEntity, m_pfnBlocked, FIELD_FUNCTION ),
END_DATADESC()

// Global Savedata for Delay
BEGIN_DATADESC( CBaseDelay )
	DEFINE_FIELD( CBaseDelay, m_flDelay, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseDelay, m_iszKillTarget, FIELD_STRING ),
END_DATADESC()

BEGIN_DATADESC( CBaseAnimating )
	DEFINE_FIELD( CBaseMonster, m_flFrameRate, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseMonster, m_flGroundSpeed, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseMonster, m_flLastEventCheck, FIELD_TIME ),
	DEFINE_FIELD( CBaseMonster, m_fSequenceFinished, FIELD_BOOLEAN ),
	DEFINE_FIELD( CBaseMonster, m_fSequenceLoops, FIELD_BOOLEAN ),
END_DATADESC()

// Global Savedata for Toggle
BEGIN_DATADESC( CBaseToggle )
	DEFINE_FIELD( CBaseToggle, m_toggle_state, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseToggle, m_flActivateFinished, FIELD_TIME ),
	DEFINE_FIELD( CBaseToggle, m_flMoveDistance, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseToggle, m_flWait, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseToggle, m_flLip, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseToggle, m_flTWidth, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseToggle, m_flTLength, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseToggle, m_vecPosition1, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseToggle, m_vecPosition2, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseToggle, m_vecAngle1, FIELD_VECTOR ),		// UNDONE: Position could go through transition, but also angle?
	DEFINE_FIELD( CBaseToggle, m_vecAngle2, FIELD_VECTOR ),		// UNDONE: Position could go through transition, but also angle?
	DEFINE_FIELD( CBaseToggle, m_cTriggersLeft, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseToggle, m_flHeight, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseToggle, m_hActivator, FIELD_EHANDLE ),
	DEFINE_FIELD( CBaseToggle, m_pfnCallWhenMoveDone, FIELD_FUNCTION ),
	DEFINE_FIELD( CBaseToggle, m_vecFinalDest, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseToggle, m_vecFinalAngle, FIELD_VECTOR ),
	DEFINE_FIELD( CBaseToggle, m_sMaster, FIELD_STRING ),
	DEFINE_FIELD( CBaseToggle, m_bitsDamageInflict, FIELD_INTEGER ),	// damage type inflicted
END_DATADESC()

// Global Savedata for monster
// UNDONE: Save schedule data?  Can this be done?  We may
// lose our enemy pointer or other data (goal ent, target, etc)
// that make the current schedule invalid, perhaps it's best
// to just pick a new one when we start up again.
BEGIN_DATADESC( CBaseMonster )
	DEFINE_FIELD( CBaseMonster, m_hEnemy, FIELD_EHANDLE ),
	DEFINE_FIELD( CBaseMonster, m_hTargetEnt, FIELD_EHANDLE ),
	DEFINE_ARRAY( CBaseMonster, m_hOldEnemy, FIELD_EHANDLE, MAX_OLD_ENEMIES ),
	DEFINE_ARRAY( CBaseMonster, m_vecOldEnemy, FIELD_POSITION_VECTOR, MAX_OLD_ENEMIES ),
	DEFINE_FIELD( CBaseMonster, m_flFieldOfView, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseMonster, m_flWaitFinished, FIELD_TIME ),
	DEFINE_FIELD( CBaseMonster, m_flMoveWaitFinished, FIELD_TIME ),
	
	DEFINE_FIELD( CBaseMonster, m_Activity, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_IdealActivity, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_LastHitGroup, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_MonsterState, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_IdealMonsterState, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_iTaskStatus, FIELD_INTEGER ),
	
	//Schedule_t			*m_pSchedule;
	
	DEFINE_FIELD( CBaseMonster, m_iScheduleIndex, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_afConditions, FIELD_INTEGER ),
	//WayPoint_t			m_Route[ ROUTE_SIZE ];
	//	DEFINE_FIELD( CBaseMonster, m_movementGoal, FIELD_INTEGER ),
	//	DEFINE_FIELD( CBaseMonster, m_iRouteIndex, FIELD_INTEGER ),
	//	DEFINE_FIELD( CBaseMonster, m_moveWaitTime, FIELD_FLOAT ),
	
	DEFINE_FIELD( CBaseMonster, m_vecMoveGoal, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseMonster, m_movementActivity, FIELD_INTEGER ),
	
	//		int					m_iAudibleList; // first index of a linked list of sounds that the monster can hear.
	//	DEFINE_FIELD( CBaseMonster, m_afSoundTypes, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_vecLastPosition, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseMonster, m_iHintNode, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_afMemory, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_iMaxHealth, FIELD_INTEGER ),
	
	DEFINE_FIELD( CBaseMonster, m_vecEnemyLKP, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseMonster, m_cAmmoLoaded, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_afCapability, FIELD_INTEGER ),
	
	DEFINE_FIELD( CBaseMonster, m_flNextAttack, FIELD_TIME ),
	DEFINE_FIELD( CBaseMonster, m_bitsDamageType, FIELD_INTEGER ),
	DEFINE_ARRAY( CBaseMonster, m_rgbTimeBasedDamage, FIELD_CHARACTER, CDMG_TIMEBASED ),
	DEFINE_FIELD( CBaseMonster, m_bloodColor, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_failSchedule, FIELD_INTEGER ),
	
	DEFINE_FIELD( CBaseMonster, m_flHungryTime, FIELD_TIME ),
	DEFINE_FIELD( CBaseMonster, m_flDistTooFar, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseMonster, m_flDistLook, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseMonster, m_iTriggerCondition, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_iszTriggerTarget, FIELD_STRING ),
	
	DEFINE_FIELD( CBaseMonster, m_HackedGunPos, FIELD_VECTOR ),
	
	DEFINE_FIELD( CBaseMonster, m_scriptState, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_pCine, FIELD_CLASSPTR ),
END_DATADESC()

BEGIN_DATADESC( CBasePlayerItem )
	DEFINE_FIELD( CBasePlayerItem, m_pPlayer, FIELD_CLASSPTR ),
	DEFINE_FIELD( CBasePlayerItem, m_pNext, FIELD_CLASSPTR ),
	//DEFINE_FIELD( CBasePlayerItem, m_fKnown, FIELD_INTEGER ),Reset to zero on load
	DEFINE_FIELD( CBasePlayerItem, m_iId, FIELD_INTEGER ),
	// DEFINE_FIELD( CBasePlayerItem, m_iIdPrimary, FIELD_INTEGER ),
	// DEFINE_FIELD( CBasePlayerItem, m_iIdSecondary, FIELD_INTEGER ),
END_DATADESC()

BEGIN_DATADESC( CBasePlayerWeapon )
#if defined( CLIENT_WEAPONS )
	DEFINE_FIELD( CBasePlayerWeapon, m_flNextPrimaryAttack, FIELD_FLOAT ),
	DEFINE_FIELD( CBasePlayerWeapon, m_flNextSecondaryAttack, FIELD_FLOAT ),
	DEFINE_FIELD( CBasePlayerWeapon, m_flTimeWeaponIdle, FIELD_FLOAT ),
#else	// CLIENT_WEAPONS
	DEFINE_FIELD( CBasePlayerWeapon, m_flNextPrimaryAttack, FIELD_TIME ),
	DEFINE_FIELD( CBasePlayerWeapon, m_flNextSecondaryAttack, FIELD_TIME ),
	DEFINE_FIELD( CBasePlayerWeapon, m_flTimeWeaponIdle, FIELD_TIME ),
#endif	// CLIENT_WEAPONS
	DEFINE_FIELD( CBasePlayerWeapon, m_iPrimaryAmmoType, FIELD_INTEGER ),
	DEFINE_FIELD( CBasePlayerWeapon, m_iSecondaryAmmoType, FIELD_INTEGER ),
	DEFINE_FIELD( CBasePlayerWeapon, m_iClip, FIELD_INTEGER ),
	DEFINE_FIELD( CBasePlayerWeapon, m_iDefaultAmmo, FIELD_INTEGER ),
	//	DEFINE_FIELD( CBasePlayerWeapon, m_iClientClip, FIELD_INTEGER )	 , reset to zero on load so hud gets updated correctly
	//  DEFINE_FIELD( CBasePlayerWeapon, m_iClientWeaponState, FIELD_INTEGER ), reset to zero on load so hud gets updated correctly
END_DATADESC()

// Global Savedata for player
BEGIN_DATADESC( CBasePlayer )
DEFINE_FIELD( CBasePlayer, m_flFlashLightTime, FIELD_TIME ),
DEFINE_FIELD( CBasePlayer, m_iFlashBattery, FIELD_INTEGER ),

DEFINE_FIELD( CBasePlayer, m_afButtonLast, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_afButtonPressed, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_afButtonReleased, FIELD_INTEGER ),

DEFINE_ARRAY( CBasePlayer, m_rgItems, FIELD_INTEGER, MAX_ITEMS ),
DEFINE_FIELD( CBasePlayer, m_afPhysicsFlags, FIELD_INTEGER ),

DEFINE_FIELD( CBasePlayer, m_flTimeStepSound, FIELD_TIME ),
DEFINE_FIELD( CBasePlayer, m_flTimeWeaponIdle, FIELD_TIME ),
DEFINE_FIELD( CBasePlayer, m_flSwimTime, FIELD_TIME ),
DEFINE_FIELD( CBasePlayer, m_flDuckTime, FIELD_TIME ),
DEFINE_FIELD( CBasePlayer, m_flWallJumpTime, FIELD_TIME ),

DEFINE_FIELD( CBasePlayer, m_flSuitUpdate, FIELD_TIME ),
DEFINE_ARRAY( CBasePlayer, m_rgSuitPlayList, FIELD_INTEGER, CSUITPLAYLIST ),
DEFINE_FIELD( CBasePlayer, m_iSuitPlayNext, FIELD_INTEGER ),
DEFINE_ARRAY( CBasePlayer, m_rgiSuitNoRepeat, FIELD_INTEGER, CSUITNOREPEAT ),
DEFINE_ARRAY( CBasePlayer, m_rgflSuitNoRepeatTime, FIELD_TIME, CSUITNOREPEAT ),
DEFINE_FIELD( CBasePlayer, m_lastDamageAmount, FIELD_INTEGER ),

DEFINE_ARRAY( CBasePlayer, m_rgpPlayerItems, FIELD_CLASSPTR, MAX_ITEM_TYPES ),
DEFINE_FIELD( CBasePlayer, m_pActiveItem, FIELD_CLASSPTR ),
DEFINE_FIELD( CBasePlayer, m_pLastItem, FIELD_CLASSPTR ),

DEFINE_ARRAY( CBasePlayer, m_rgAmmo, FIELD_INTEGER, MAX_AMMO_SLOTS ),
DEFINE_FIELD( CBasePlayer, m_idrowndmg, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_idrownrestored, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_tSneaking, FIELD_TIME ),

DEFINE_FIELD( CBasePlayer, m_iTrain, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_bitsHUDDamage, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_flFallVelocity, FIELD_FLOAT ),
DEFINE_FIELD( CBasePlayer, m_iTargetVolume, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_iWeaponVolume, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_iExtraSoundTypes, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_iWeaponFlash, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_fLongJump, FIELD_BOOLEAN ),
DEFINE_FIELD( CBasePlayer, m_fInitHUD, FIELD_BOOLEAN ),
DEFINE_FIELD( CBasePlayer, m_tbdPrev, FIELD_TIME ),

DEFINE_FIELD( CBasePlayer, m_pTank, FIELD_EHANDLE ),
DEFINE_FIELD( CBasePlayer, m_iHideHUD, FIELD_INTEGER ),
DEFINE_FIELD( CBasePlayer, m_iFOV, FIELD_INTEGER ),

//DEFINE_FIELD( CBasePlayer, m_fDeadTime, FIELD_FLOAT ), // only used in multiplayer games
//DEFINE_FIELD( CBasePlayer, m_fGameHUDInitialized, FIELD_INTEGER ), // only used in multiplayer games
//DEFINE_FIELD( CBasePlayer, m_flStopExtraSoundTime, FIELD_TIME ),
//DEFINE_FIELD( CBasePlayer, m_bKnownItem, FIELD_BOOLEAN ), // reset to zero on load
//DEFINE_FIELD( CBasePlayer, m_iPlayerSound, FIELD_INTEGER ),	// Don't restore, set in Precache()
//DEFINE_FIELD( CBasePlayer, m_pentSndLast, FIELD_EDICT ),	// Don't restore, client needs reset
//DEFINE_FIELD( CBasePlayer, m_flSndRoomtype, FIELD_FLOAT ),	// Don't restore, client needs reset
//DEFINE_FIELD( CBasePlayer, m_flSndRange, FIELD_FLOAT ),	// Don't restore, client needs reset
//DEFINE_FIELD( CBasePlayer, m_fNewAmmo, FIELD_INTEGER ), // Don't restore, client needs reset
//DEFINE_FIELD( CBasePlayer, m_flgeigerRange, FIELD_FLOAT ),	// Don't restore, reset in Precache()
//DEFINE_FIELD( CBasePlayer, m_flgeigerDelay, FIELD_FLOAT ),	// Don't restore, reset in Precache()
//DEFINE_FIELD( CBasePlayer, m_igeigerRangePrev, FIELD_FLOAT ),	// Don't restore, reset in Precache()
//DEFINE_FIELD( CBasePlayer, m_iStepLeft, FIELD_INTEGER ), // Don't need to restore
//DEFINE_ARRAY( CBasePlayer, m_szTextureName, FIELD_CHARACTER, CBTEXTURENAMEMAX ), // Don't need to restore
//DEFINE_FIELD( CBasePlayer, m_chTextureType, FIELD_CHARACTER ), // Don't need to restore
//DEFINE_FIELD( CBasePlayer, m_fNoPlayerSound, FIELD_BOOLEAN ), // Don't need to restore, debug
//DEFINE_FIELD( CBasePlayer, m_iUpdateTime, FIELD_INTEGER ), // Don't need to restore
//DEFINE_FIELD( CBasePlayer, m_iClientHealth, FIELD_INTEGER ), // Don't restore, client needs reset
//DEFINE_FIELD( CBasePlayer, m_iClientBattery, FIELD_INTEGER ), // Don't restore, client needs reset
//DEFINE_FIELD( CBasePlayer, m_iClientHideHUD, FIELD_INTEGER ), // Don't restore, client needs reset
//DEFINE_FIELD( CBasePlayer, m_fWeapon, FIELD_BOOLEAN ),  // Don't restore, client needs reset
//DEFINE_FIELD( CBasePlayer, m_nCustomSprayFrames, FIELD_INTEGER ), // Don't restore, depends on server message after spawning and only matters in multiplayer
//DEFINE_FIELD( CBasePlayer, m_vecAutoAim, FIELD_VECTOR ), // Don't save/restore - this is recomputed
//DEFINE_ARRAY( CBasePlayer, m_rgAmmoLast, FIELD_INTEGER, MAX_AMMO_SLOTS ), // Don't need to restore
//DEFINE_FIELD( CBasePlayer, m_fOnTarget, FIELD_BOOLEAN ), // Don't need to restore
//DEFINE_FIELD( CBasePlayer, m_nCustomSprayFrames, FIELD_INTEGER ), // Don't need to restore
END_DATADESC()