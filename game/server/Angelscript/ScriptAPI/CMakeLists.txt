add_sources(
	ASGameRules.h
	ASGameRules.cpp
)

add_subdirectory( Entities )
add_subdirectory( Extensions )

if( USE_AS_SQL )
	add_subdirectory( SQL )
endif()