add_sources(
	CClientVGUI.h
	CClientVGUI.cpp
)

add_subdirectory( hud )
add_subdirectory( vgui )

if( USE_VGUI2 )
	add_subdirectory( vgui2 )
endif()
