add_sources(
	ASGameRules.h
	ASGameRules.cpp
)

add_subdirectory( Extensions )

if( "${USE_AS_SQL}" STREQUAL 1 )
	add_subdirectory( SQL )
endif()