#ifndef COMMON_PLATFORM_H
#define COMMON_PLATFORM_H

/*
*	Platform specific code compatibility.
*/

#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstdlib>
#include <cstring>

#ifdef WIN32

// Prevent tons of unused windows definitions
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#define NOMINMAX
#include "winsani_in.h"
#include "windows.h"
#include "winsani_out.h"

#define stricmp _stricmp
#define strupr _strupr
#define stricmp _stricmp
#define strnicmp _strnicmp

#define MakeDirectory( pszDirectory ) CreateDirectoryA( pszDirectory, nullptr )

//Define these macros once only. The SDK is such a mess. - Solokiller
#define DLLEXPORT __declspec( dllexport )
#define DLLIMPORT __declspec( dllimport )

#else

#define MAX_PATH PATH_MAX

//mkdir
#include <unistd.h>

#ifndef _vsnprintf
#define _vsnprintf(a,b,c,d) vsnprintf(a,b,c,d)
#endif

//TODO: verify that the mode parameter is correct - Solokiller
#define MakeDirectory( pszDirectory ) mkdir( pszDirectory, 0777 )

#define DLLEXPORT __attribute__( ( visibility( "default" ) ) )
#define DLLIMPORT

#endif

#endif //COMMON_PLATFORM_H