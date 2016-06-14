//TODO: make one header that includes the platform headers - Solokiller
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Platform.h"

#ifdef _WIN32

// Required DLL entry point
BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,
	DWORD fdwReason,
	LPVOID lpvReserved )
{
	if( fdwReason == DLL_PROCESS_ATTACH )
	{
	}
	else if( fdwReason == DLL_PROCESS_DETACH )
	{
	}
	return TRUE;
}
#endif