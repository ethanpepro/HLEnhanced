add_sources(
	animation.h
	animation.cpp
	ButtonSounds.h
	ButtonSounds.cpp
	CGlobalState.h
	CGlobalState.cpp
	client.h
	client.cpp
	CMap.h
	CMap.cpp
	CMultiDamage.h
	CMultiDamage.cpp
	CServerGameInterface.h
	CServerGameInterface.cpp
	CStudioBlending.h
	CStudioBlending.cpp
	Decals.h
	Decals.cpp
	Effects.h
	globals.cpp
	KeyValuesCompat.cpp
	MapCycle.h
	MapCycle.cpp
	SaveRestore.h
	Server.h
	Server.cpp
	ServerEngineOverride.h
	ServerEngineOverride.cpp
	ServerInterface.h
	ServerInterface.cpp
	Skill.h
	Skill.cpp
	SVC.h
	TempEntity.h
	TempEntity.cpp
	UserMessages.h
	UserMessages.cpp
	util.cpp
	voice_gamemgr.h
	voice_gamemgr.cpp
	Weapons.cpp
)

add_subdirectory( config )
add_subdirectory( engine )
add_subdirectory( entities )
add_subdirectory( gamerules )
add_subdirectory( nodes )
add_subdirectory( saverestore )
add_subdirectory( sound )
