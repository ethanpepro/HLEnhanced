add_sources(
	ASCSentenceGroups.h
	ASCSentenceGroups.cpp
	ASCServerEngine.h
	ASCServerEngine.cpp
	ASCSoundSystem.h
	ASCSoundSystem.cpp
	ASCustomEntities.h
	ASCustomEntities.cpp
	ASEffects.h
	ASEffects.cpp
	ASGameRules.h
	ASGameRules.cpp
	ASPluginInterface.h
	ASPluginInterface.cpp
	ASTriggerScript.h
	ASTriggerScript.cpp
	ASUtilityFuncs.h
	ASUtilityFuncs.cpp
	CASSayArgs.h
	CASSayArgs.cpp
)

add_subdirectory( Console )
add_subdirectory( Entities )
add_subdirectory( Extensions )

if( USE_AS_SQL )
	add_subdirectory( SQL )
endif()