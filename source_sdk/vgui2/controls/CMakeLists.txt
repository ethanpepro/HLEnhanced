###################################################
#                                                 #
#                                                 #
#	VGUI2 Controls CMake build file               #
#                                                 #
#                                                 #
###################################################

add_sources(
	animatingimagepanel.cpp
	animationcontroller.cpp
	bitmapimagepanel.cpp
	buildfactoryhelper.cpp
	buildgroup.cpp
	buildmodedialog.cpp
	button.cpp
	checkbutton.cpp
	checkbuttonlist.cpp
	combobox.cpp
	controls.cpp
	directoryselectdialog.cpp
	divider.cpp
	editablepanel.cpp
	expandbutton.cpp
	fileopendialog.cpp
	focusnavgroup.cpp
	frame.cpp
	graphpanel.cpp
	html.cpp
	image.cpp
	imagelist.cpp
	imagepanel.cpp
	inputdialog.cpp
	keybindinghelpdialog.cpp
	keyboardeditordialog.cpp
	label.cpp
	listpanel.cpp
	listviewpanel.cpp
	menu.cpp
	menubar.cpp
	menubutton.cpp
	menuitem.cpp
	messagebox.cpp
	panel.cpp
	panellistpanel.cpp
	progressbar.cpp
	progressbox.cpp
	propertydialog.cpp
	propertypage.cpp
	propertysheet.cpp
	querybox.cpp
	radiobutton.cpp
	richtext.cpp
	scrollbar.cpp
	scrollbarslider.cpp
	sectionedlistpanel.cpp
	slider.cpp
	splitter.cpp
	textentry.cpp
	textimage.cpp
	togglebutton.cpp
	tooltip.cpp
	toolwindow.cpp
	treeview.cpp
	treeviewlistcontrol.cpp
	urllabel.cpp
	wizardpanel.cpp
	wizardsubpanel.cpp
)

preprocess_sources()

include_directories(
	${CMAKE_SOURCE_DIR}/source_sdk/public
	${CMAKE_SOURCE_DIR}/source_sdk/public/tier1
	${CMAKE_SOURCE_DIR}/public
	${CMAKE_SOURCE_DIR}
)

add_library( vgui2_controls STATIC ${PREP_SRCS} )

target_compile_definitions( vgui2_controls PRIVATE
	NO_MALLOC_OVERRIDE
)

set_target_properties( vgui2_controls 
	PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" 
	LINK_FLAGS "${LINUX_32BIT_FLAG}" )
	
	#No lib prefix
SET_TARGET_PROPERTIES( vgui2_controls PROPERTIES PREFIX "" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

clear_sources()
