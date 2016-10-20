add_sources(
	ASGameRules.h
	ASGameRules.cpp
	ASPluginInterface.h
	ASPluginInterface.cpp
)

add_subdirectory( Entities )
add_subdirectory( Extensions )

if( USE_AS_SQL )
	add_subdirectory( SQL )
endif()