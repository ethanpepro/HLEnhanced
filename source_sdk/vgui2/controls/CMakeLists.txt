add_sources(
	AnimatingImagePanel.cpp
	AnimationController.cpp
	BitmapImagePanel.cpp
	BuildFactoryHelper.cpp
	BuildGroup.cpp
	BuildModeDialog.cpp
	Button.cpp
	CheckButton.cpp
	CheckButtonList.cpp
	ComboBox.cpp
	Controls.cpp
	DirectorySelectDialog.cpp
	Divider.cpp
	EditablePanel.cpp
	ExpandButton.cpp
	FileOpenDialog.cpp
	FocusNavGroup.cpp
	Frame.cpp
	GraphPanel.cpp
	HTML.cpp
	Image.cpp
	ImageList.cpp
	ImagePanel.cpp
	InputDialog.cpp
	KeyBindingHelpDialog.cpp
	KeyBoardEditorDialog.cpp
	Label.cpp
	ListPanel.cpp
	ListViewPanel.cpp
	MenuBar.cpp
	MenuButton.cpp
	Menu.cpp
	MenuItem.cpp
	MessageBox.cpp
	Panel.cpp
	PanelListPanel.cpp
	ProgressBar.cpp
	ProgressBox.cpp
	PropertyDialog.cpp
	PropertyPage.cpp
	PropertySheet.cpp
	QueryBox.cpp
	RadioButton.cpp
	RichText.cpp
	ScrollBar.cpp
	ScrollBarSlider.cpp
	SectionedListPanel.cpp
	Slider.cpp
	Splitter.cpp
	TextEntry.cpp
	TextImage.cpp
	ToggleButton.cpp
	Tooltip.cpp
	ToolWindow.cpp
	TreeView.cpp
	TreeViewListControl.cpp
	URLLabel.cpp
	WizardPanel.cpp
	WizardSubPanel.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/source_sdk/common source_sdk/common)
add_subdirectory(${CMAKE_SOURCE_DIR}/source_sdk/public source_sdk/public)
add_subdirectory(${CMAKE_SOURCE_DIR}/source_sdk/vgui2/src source_sdk/vgui2/src)

preprocess_sources()

add_library(vgui2_controls STATIC ${PREP_SRCS})

set_common_properties(vgui2_controls)

target_include_directories(vgui2_controls PRIVATE
	${CMAKE_SOURCE_DIR}/source_sdk/common
	${CMAKE_SOURCE_DIR}/source_sdk/public
	${CMAKE_SOURCE_DIR}/source_sdk/public/html
	${CMAKE_SOURCE_DIR}/source_sdk/public/tier1
	${CMAKE_SOURCE_DIR}/common
	${CMAKE_SOURCE_DIR}/public
${CMAKE_SOURCE_DIR}) # Reverse order

clear_sources()

get_target_property(VGUI2_CONTROLS_SOURCES vgui2_controls SOURCES)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${VGUI2_CONTROLS_SOURCES})
