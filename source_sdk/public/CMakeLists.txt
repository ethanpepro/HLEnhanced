add_sources(
	basetypes.h
	commonmacros.h
	coordsize.h
	datamap.h
	filesystem_helpers.cpp
	filesystem_helpers.h
	icvar.h
	mathlib.h
	OfflineMode.h
	sdk_color.h
	vector.h
	vector2d.h
	winlite.h
	worldsize.h
)

add_subdirectory( appframework )
add_subdirectory( bitmap )
add_subdirectory( mathlib )
add_subdirectory( tier0 )
add_subdirectory( tier1 )
add_subdirectory( vgui )
add_subdirectory( vgui_controls )
add_subdirectory( vstdlib )
