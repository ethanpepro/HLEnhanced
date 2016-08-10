add_sources(
	cbase.h
	CBaseGameInterface.h
	CBaseGameInterface.cpp
	cdll_dll.h
	CTakeDamageInfo.h
	CWeaponInfo.h
	CWeaponInfo.cpp
	CWeaponInfoCache.h
	CWeaponInfoCache.cpp
	Damage.h
	enginecallback.h
	extdll.h
	GameConstants.h
	ScriptEvent.h
	shared_game_utils.h
	shared_game_utils.cpp
	util.h
	voice_common.h
	Weapons.h
	Weapons_Shared.cpp
	WeaponsConst.h
	WeaponsConst.cpp
	Weather.h
)

if( USE_ANGELSCRIPT )
	add_subdirectory( Angelscript )
endif()
add_subdirectory( entities )
add_subdirectory( materials )