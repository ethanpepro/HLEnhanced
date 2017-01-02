###################################################
#                                                 #
#                                                 #
#	Tier1 CMake build file                        #
#                                                 #
#                                                 #
###################################################

add_sources(
	bitbuf.cpp
	byteswap.cpp
	characterset.cpp
	checksum_crc.cpp
	checksum_md5.cpp
	convar.cpp
	datamanager.cpp
	diff.cpp
	generichash.cpp
	#interface.cpp
	jobthread.cpp
	keyvalues.cpp
	mempool.cpp
	memstack.cpp
	netadr.cpp
	processor_detect.cpp
	rangecheckedvar.cpp
	stringpool.cpp
	strtools.cpp
	tier1.cpp
	tokenreader.cpp
	utlbuffer.cpp
	utlstring.cpp
	utlsymbol.cpp
	xboxstubs.cpp
)

if( UNIX )
	add_sources(
		processor_detect_linux.cpp
	)
endif()

preprocess_sources()

include_directories(
	${CMAKE_SOURCE_DIR}/source_sdk/public
	${CMAKE_SOURCE_DIR}/source_sdk/public/tier1
	${CMAKE_SOURCE_DIR}/source_sdk/common
	${CMAKE_SOURCE_DIR}/common
	${CMAKE_SOURCE_DIR}/public
	${CMAKE_SOURCE_DIR}
)

add_library( tier1 STATIC ${PREP_SRCS} )

target_compile_definitions( tier1 PRIVATE
	${SHARED_DEFS}
	TIER1_STATIC_LIB
	NO_MALLOC_OVERRIDE
)

set_target_properties( tier1 
	PROPERTIES COMPILE_FLAGS "${LINUX_32BIT_FLAG}" 
	LINK_FLAGS "${LINUX_32BIT_FLAG}" )
	
	#No lib prefix
SET_TARGET_PROPERTIES( tier1 PROPERTIES PREFIX "" )

#Create filters
create_source_groups( "${CMAKE_SOURCE_DIR}" )

clear_sources()
