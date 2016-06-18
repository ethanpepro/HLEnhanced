/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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

/*
==========================
This file contains "stubs" of class member implementations so that we can predict certain
 weapons client side.  From time to time you might find that you need to implement part of the
 these functions.  If so, cut it from here, paste it in hl_weapons.cpp or somewhere else and
 add in the functionality you need.
==========================
*/
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"entities/CBasePlayer.h"
#include	"weapons.h"
#include	"nodes.h"
#include	"soundent.h"
#include	"skill.h"

// Globals used by game logic
const Vector g_vecZero = Vector( 0, 0, 0 );
int gmsgWeapPickup = 0;
enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals;

ItemInfo CBasePlayerItem::ItemInfoArray[MAX_WEAPONS];

void EMIT_SOUND_DYN(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int flags, int pitch) { }

// CBaseDelay Stubs
void CBaseDelay :: KeyValue( struct KeyValueData_s * ) { }

// DEBUG Stubs
edict_t *DBG_EntOfVars( const entvars_t *pev ) { return NULL; }
void DBG_AssertFunction(const bool fExpr, const char* szExpr, const char* szFile, int szLine, const char* szMessage) { }

// UTIL_* Stubs
void UTIL_PrecacheOther( const char *szClassname ) { }
void UTIL_BloodDrips( const Vector &origin, const Vector &direction, int color, int amount ) { }
void UTIL_DecalTrace( TraceResult *pTrace, int decalNumber ) { }
void UTIL_GunshotDecalTrace( TraceResult *pTrace, int decalNumber ) { }
void UTIL_MakeVectors( const Vector &vecAngles ) { }
bool UTIL_IsValidEntity( edict_t *pent ) { return true; }
void UTIL_SetOrigin( entvars_t *, const Vector &org ) { }
bool UTIL_GetNextBestWeapon( CBasePlayer *pPlayer, CBasePlayerItem *pCurrentWeapon ) { return true; }
void UTIL_LogPrintf(char *,...) { }
void UTIL_ClientPrintAll( int,char const *,char const *,char const *,char const *,char const *) { }
void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 ) { }

// CBaseToggle Stubs
void CBaseToggle :: KeyValue( struct KeyValueData_s * ) { }
bool CBaseToggle::IsLockedByMaster() const { return false; }

// CGrenade Stubs
void CGrenade::BounceSound( void ) { }
void CGrenade::Explode( Vector, Vector ) { }
void CGrenade::Explode( TraceResult *, int ) { }
void CGrenade::Killed( entvars_t *, GibAction gibAction ) { }
void CGrenade::Spawn( void ) { }
CGrenade * CGrenade:: ShootTimed( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float time ){ return 0; }
CGrenade *CGrenade::ShootContact( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity ){ return 0; }
void CGrenade::DetonateUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ){ }

void UTIL_Remove( CBaseEntity *pEntity ){ }
struct skilldata_t  gSkillData;
void UTIL_SetSize( entvars_t *pev, const Vector &vecMin, const Vector &vecMax ){ }
CBaseEntity *UTIL_FindEntityInSphere( CBaseEntity *pStartEntity, const Vector &vecCenter, float flRadius ){ return 0;}

Vector UTIL_VecToAngles( const Vector &vec ){ return 0; }
CSprite *CSprite::SpriteCreate( const char *pSpriteName, const Vector &origin, const bool animate ) { return 0; }
void CBeam::PointEntInit( const Vector &start, int endIndex ) { }
CBeam *CBeam::BeamCreate( const char *pSpriteName, int width ) { return NULL; }
void CSprite::Expand( float scaleSpeed, float fadeSpeed ) { }


CBaseEntity* CBaseMonster :: CheckTraceHullAttack( float flDist, int iDamage, int iDmgType ) { return NULL; }
void CBaseMonster :: Eat ( float flFullDuration ) { }
bool CBaseMonster::FShouldEat() const { return true; }
void CBaseMonster::BarnacleVictimBitten( CBaseEntity* pBarnacle ) {}
void CBaseMonster :: BarnacleVictimReleased ( void ) { }
void CBaseMonster :: Listen ( void ) { }
float CBaseMonster :: FLSoundVolume ( CSound *pSound ) { return 0.0; }
bool CBaseMonster::FValidateHintType( short sHint ) const { return false; }
void CBaseMonster :: Look ( int iDistance ) { }
int CBaseMonster :: ISoundMask ( void ) { return 0; }
CSound* CBaseMonster :: PBestSound ( void ) { return NULL; }
CSound* CBaseMonster :: PBestScent ( void ) { return NULL; } 
float CBaseAnimating :: StudioFrameAdvance ( float flInterval ) { return 0.0; }
void CBaseMonster :: MonsterThink ( void ) { }
void CBaseMonster :: MonsterUse ( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value ) { }
int CBaseMonster :: IgnoreConditions ( void ) { return 0; }
void CBaseMonster :: RouteClear ( void ) { }
void CBaseMonster :: RouteNew ( void ) { }
bool CBaseMonster::FRouteClear() const { return false; }
bool CBaseMonster::FRefreshRoute() { return false; }
bool CBaseMonster::MoveToEnemy( Activity movementAct, float waitTime ) { return false; }
bool CBaseMonster::MoveToLocation( Activity movementAct, float waitTime, const Vector &goal ) { return false; }
bool CBaseMonster::MoveToTarget( Activity movementAct, float waitTime ) { return false; }
bool CBaseMonster::MoveToNode( Activity movementAct, float waitTime, const Vector &goal ) { return false; }
//TODO: why is this even here? - Solokiller
bool ShouldSimplify( int routeType ) { return true; }
void CBaseMonster::RouteSimplify( const CBaseEntity* const pTargetEnt ) { }
bool CBaseMonster :: FBecomeProne() { return true; }
bool CBaseMonster::CheckRangeAttack1( float flDot, float flDist ) { return false; }
bool CBaseMonster::CheckRangeAttack2( float flDot, float flDist ) { return false; }
bool CBaseMonster::CheckMeleeAttack1( float flDot, float flDist ) { return false; }
bool CBaseMonster::CheckMeleeAttack2( float flDot, float flDist ) { return false; }
void CBaseMonster :: CheckAttacks ( CBaseEntity *pTarget, float flDist ) { }
bool CBaseMonster::FCanCheckAttacks() const { return false; }
bool CBaseMonster::CheckEnemy( CBaseEntity *pEnemy ) { return false; }
void CBaseMonster :: PushEnemy( CBaseEntity *pEnemy, Vector &vecLastKnownPos ) { }
bool CBaseMonster::PopEnemy() { return false; }
void CBaseMonster :: SetActivity ( Activity NewActivity ) { }
void CBaseMonster :: SetSequenceByName ( char *szSequence ) { }
int CBaseMonster::CheckLocalMove( const Vector &vecStart, const Vector &vecEnd, const CBaseEntity* const pTarget, float *pflDist ) { return 0; }
float CBaseMonster :: OpenDoorAndWait( entvars_t *pevDoor ) { return 0.0; }
void CBaseMonster :: AdvanceRoute ( float distance ) { }
int CBaseMonster :: RouteClassify( int iMoveFlag ) { return 0; }
bool CBaseMonster::BuildRoute( const Vector &vecGoal, int iMoveFlag, const CBaseEntity* const pTarget ) { return false; }
void CBaseMonster :: InsertWaypoint ( Vector vecLocation, int afMoveFlags ) { }
bool CBaseMonster::FTriangulate( const Vector &vecStart , const Vector &vecEnd, float flDist, const CBaseEntity* const pTargetEnt, Vector *pApex ) { return false; }
void CBaseMonster :: Move ( float flInterval ) { }
bool CBaseMonster::ShouldAdvanceRoute( float flWaypointDist ) { return false; }
void CBaseMonster::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval ) { }
void CBaseMonster :: MonsterInit ( void ) { }
void CBaseMonster :: MonsterInitThink ( void ) { }
void CBaseMonster :: StartMonster ( void ) { }
void CBaseMonster :: MovementComplete( void ) { }
bool CBaseMonster::TaskIsRunning() const { return false; }
int CBaseMonster::IRelationship ( CBaseEntity *pTarget ) { return 0; }
bool CBaseMonster::FindCover( Vector vecThreat, Vector vecViewOffset, float flMinDist, float flMaxDist ) { return false; }
bool CBaseMonster::BuildNearestRoute( Vector vecThreat, Vector vecViewOffset, float flMinDist, float flMaxDist ) { return false; }
CBaseEntity *CBaseMonster :: BestVisibleEnemy ( void ) { return NULL; }
bool CBaseMonster::FInViewCone( const CBaseEntity *pEntity ) const { return false; }
bool CBaseMonster::FInViewCone( const Vector& vecOrigin ) const { return false; }
void CBaseMonster :: MakeIdealYaw( Vector vecTarget ) { }
float CBaseMonster::FlYawDiff() const { return 0.0; }
float CBaseMonster::ChangeYaw ( int yawSpeed ) { return 0; }
float	CBaseMonster::VecToYaw ( Vector vecDir ) { return 0.0; }
int CBaseAnimating :: LookupActivity ( int activity ) { return 0; }
int CBaseAnimating :: LookupActivityHeaviest ( int activity ) { return 0; }
void CBaseMonster :: SetEyePosition ( void ) { }
int CBaseAnimating :: LookupSequence ( const char *label ) { return 0; }
void CBaseAnimating :: ResetSequenceInfo ( ) { }
int CBaseAnimating::GetSequenceFlags() const { return 0; }
void CBaseAnimating :: DispatchAnimEvents ( float flInterval ) { }
void CBaseMonster :: HandleAnimEvent( MonsterEvent_t *pEvent ) { }
float CBaseAnimating :: SetBoneController ( int iController, float flValue ) { return 0.0; }
void CBaseAnimating :: InitBoneControllers ( void ) { }
float CBaseAnimating :: SetBlending ( int iBlender, float flValue ) { return 0; }
void CBaseAnimating :: GetBonePosition ( int iBone, Vector &origin, Vector &angles ) { }
void CBaseAnimating :: GetAttachment ( int iAttachment, Vector &origin, Vector &angles ) { }
int CBaseAnimating :: FindTransition( int iEndingSequence, int iGoalSequence, int *piDir ) { return -1; }
void CBaseAnimating :: GetAutomovement( Vector &origin, Vector &angles, float flInterval ) { }
void CBaseAnimating :: SetBodygroup( int iGroup, int iValue ) { }
int CBaseAnimating :: GetBodygroup( int iGroup ) { return 0; }
Vector CBaseMonster :: GetGunPosition( void ) { return g_vecZero; }

void CBaseMonster :: MakeDamageBloodDecal ( int cCount, float flNoise, TraceResult *ptr, const Vector &vecDir ) { }
bool CBaseMonster::FGetNodeRoute( const Vector& vecDest ) { return true; }
int CBaseMonster :: FindHintNode ( void ) { return NO_NODE; }
void CBaseMonster::ReportAIState( void ) { }
void CBaseMonster :: KeyValue( KeyValueData *pkvd ) { }
bool CBaseMonster::FCheckAITrigger() { return false; }
bool CBaseMonster :: CanPlaySequence( const bool fDisregardMonsterState, int interruptLevel ) const { return false; }
bool CBaseMonster::FindLateralCover( const Vector &vecThreat, const Vector &vecViewOffset ) { return false; }
Vector CBaseMonster :: ShootAtEnemy( const Vector &shootOrigin ) { return g_vecZero; }
bool CBaseMonster::FacingIdeal() const { return false; }
bool CBaseMonster::FCanActiveIdle() const { return false; }
void CBaseMonster::PlaySentence( const char *pszSentence, float duration, float volume, float attenuation ) { }
void CBaseMonster::PlayScriptedSentence( const char *pszSentence, float duration, float volume, float attenuation, const bool bConcurrent, CBaseEntity *pListener ) { }
void CBaseMonster::SentenceStop( void ) { }
void CBaseMonster::CorpseFallThink( void ) { }
void CBaseMonster :: MonsterInitDead( void ) { }
bool CBaseMonster::BBoxFlat() const { return true; }
bool CBaseMonster::GetEnemy() { return false; }
void CBaseMonster :: TraceAttack( entvars_t *pevAttacker, float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType) { }
CBaseEntity* CBaseMonster :: DropItem ( char *pszItemName, const Vector &vecPos, const Vector &vecAng ) { return NULL; }
bool CBaseMonster::ShouldFadeOnDeath() const { return false; }
void CBaseMonster::RadiusDamage( CBaseEntity* pInflictor, CBaseEntity* pAttacker, float flDamage, int iClassIgnore, int bitsDamageType ) {}
void CBaseMonster::RadiusDamage( Vector vecSrc, CBaseEntity* pInflictor, CBaseEntity* pAttacker, float flDamage, int iClassIgnore, int bitsDamageType ) {}
void CBaseMonster::FadeMonster( void ) { }
void CBaseMonster :: GibMonster( void ) { }
bool CBaseMonster::HasHumanGibs() { return false; }
bool CBaseMonster::HasAlienGibs() { return false; }
Activity CBaseMonster :: GetDeathActivity ( void ) { return ACT_DIE_HEADSHOT; }
MONSTERSTATE CBaseMonster :: GetIdealState ( void ) { return MONSTERSTATE_ALERT; }
Schedule_t* CBaseMonster :: GetScheduleOfType ( int Type ) { return NULL; }
Schedule_t *CBaseMonster :: GetSchedule ( void ) { return NULL; }
void CBaseMonster :: RunTask ( Task_t *pTask ) { }
void CBaseMonster :: StartTask ( Task_t *pTask ) { }
Schedule_t *CBaseMonster::ScheduleFromName( const char *pName ) { return NULL;}
void CBaseMonster::BecomeDead( void ) {}
void CBaseMonster :: RunAI ( void ) {}
void CBaseMonster::Killed( entvars_t *pevAttacker, GibAction gibAction ) {}
float CBaseMonster::GiveHealth( float flHealth, int bitsDamageType ) { return 0; }
int CBaseMonster::TakeDamage( CBaseEntity* pInflictor, CBaseEntity* pAttacker, float flDamage, int bitsDamageType ) { return 0; }
bool CBaseMonster::Restore( CRestore& ) { return true; }

int TrainSpeed(int iSpeed, int iMax) { 	return 0; }
CBaseEntity *FindEntityForward( CBaseEntity *pMe ) { return NULL; }

void SpawnBlood(Vector vecSpot, int bloodColor, float flDamage) { }
int DamageDecal( CBaseEntity *pEntity, int bitsDamageType ) { return 0; }
void DecalGunshot( TraceResult *pTrace, int iBulletType ) { }
void EjectBrass ( const Vector &vecOrigin, const Vector &vecVelocity, float rotation, int model, int soundtype ) { }
void CBasePlayerAmmo::Spawn( void ) { }
CBaseEntity* CBasePlayerAmmo::Respawn( void ) { return this; }
void CBasePlayerAmmo::Materialize( void ) { }
void CBasePlayerAmmo :: DefaultTouch( CBaseEntity *pOther ) { }
void CSoundEnt::InsertSound ( int iType, const Vector &vecOrigin, int iVolume, float flDuration ) {}
void RadiusDamage( Vector vecSrc, CBaseEntity* pInflictor, CBaseEntity* pAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType ) {}

bool UTIL_GiveAmmoToPlayer( CBaseEntity* pGiver, CBaseEntity* pPlayer,
							const int iAmount, const char* const pszAmmoName,
							const char* const pszPickupSound )
{
	return false;
}