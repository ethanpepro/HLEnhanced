add_sources(
	bitbuf.cpp
	byteswap.cpp
	CharacterSet.cpp
	checksum_crc.cpp
	checksum_md5.cpp
	ConVar.cpp
	DataManager.cpp
	Diff.cpp
	generichash.cpp
	#interface.cpp
	JobThread.cpp
	KeyValues.cpp
	MemPool.cpp
	MemStack.cpp
	netadr.cpp
	processor_detect.cpp
	RangeCheckedVar.cpp
	StringPool.cpp
	strtools.cpp
	tier1.cpp
	TokenReader.cpp
	UtlBuffer.cpp
	UtlString.cpp
	UtlSymbol.cpp
	xboxstubs.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/source_sdk/common source_sdk/common)
add_subdirectory(${CMAKE_SOURCE_DIR}/source_sdk/public source_sdk/public)

preprocess_sources()

add_library(tier1 STATIC ${PREP_SRCS})

set_common_properties(tier1)

target_include_directories(tier1 PRIVATE
	${CMAKE_SOURCE_DIR}/source_sdk/common
	${CMAKE_SOURCE_DIR}/source_sdk/public
	${CMAKE_SOURCE_DIR}/source_sdk/public/tier1
	${CMAKE_SOURCE_DIR}/common
	${CMAKE_SOURCE_DIR}/public
	${CMAKE_SOURCE_DIR}) # Reverse order

target_compile_definitions(tier1 PRIVATE
	TIER1_STATIC_LIB)

clear_sources()

get_target_property(TIER1_SOURCES tier1 SOURCES)

source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${TIER1_SOURCES})
