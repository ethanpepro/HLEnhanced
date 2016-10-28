add_sources(
	ASCustomEntities.h
	ASCustomEntities.cpp
	ASGameRules.h
	ASGameRules.cpp
	ASPluginInterface.h
	ASPluginInterface.cpp
	ASTriggerScript.h
	ASTriggerScript.cpp
)

add_subdirectory( Entities )
add_subdirectory( Extensions )

if( USE_AS_SQL )
	add_subdirectory( SQL )
endif()