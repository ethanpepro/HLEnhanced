add_sources(
	basetypes.h
	color.h
	filesystem_helpers.h
	filesystem_helpers.cpp
	mathlib.h
	offlinemode.h
	vector.h
	vector2D.h
	winlite.h
)

add_subdirectory( appframework )
add_subdirectory( bitmap )
add_subdirectory( mathlib )
add_subdirectory( tier0 )
add_subdirectory( tier1 )
add_subdirectory( vgui )
add_subdirectory( vgui_controls )
add_subdirectory( vstdlib )
