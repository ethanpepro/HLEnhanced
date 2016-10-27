add_sources(
	ASPluginConstants.h
	ASPluginConstants.cpp
	CASPluginData.h
	CASPluginManager.h
	CASPluginManager.cpp
	CASPluginModuleBuilder.h
	CASPluginModuleBuilder.cpp
	CHLASServerInitializer.h
	CHLASServerInitializer.cpp
	CHLASServerManager.h
	CHLASServerManager.cpp
)

add_subdirectory( Entities )
add_subdirectory( ScriptAPI )
