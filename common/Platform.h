#ifndef COMMON_PLATFORM_H
#define COMMON_PLATFORM_H

/*
*	Platform specific code compatibility.
*/

// Misc C-runtime library headers
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdarg>
#include <cstdio>
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

//Older versions of Visual Studio (< VS2015) didn't define these. - Solokiller
#if _MSC_VER < 1900
#define snprintf _snprintf
#define vsnprintf _vsnprintf
//Provided by stdlib in VS2015 - Solokiller
//TODO: needed on Linux?
#define fabs(x) ((x) > 0 ? (x) : 0 - (x))
#endif

#define MakeDirectory( pszDirectory ) CreateDirectoryA( pszDirectory, nullptr )

//Define these macros once only. The SDK is such a mess. - Solokiller
#define DLLEXPORT __declspec( dllexport )
#define DLLIMPORT __declspec( dllimport )

//This isn't needed 99.99% of the time. Provide an escape hatch nonetheless. - Solokiller
#ifndef HL_USE_VOID_DEF
#undef VOID
#endif

#else

//mkdir
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define MAX_PATH PATH_MAX

#ifndef _vsnprintf
#define _vsnprintf(a,b,c,d) vsnprintf(a,b,c,d)
#endif

#define MakeDirectory( pszDirectory ) mkdir( pszDirectory, 0777 )

#define DLLEXPORT __attribute__( ( visibility( "default" ) ) )
#define DLLIMPORT

#define __cdecl

//TODO: GLM has ivec2, use that. - Solokiller
#ifdef LINUX
typedef struct POINT_s
{
	int x;
	int y;
} POINT;
#endif

//TODO: Should handle windows too - Solokiller
#if defined(OSX)
#define SO_ARCH_SUFFIX ".dylib"
#else
#if defined ( __x86_64__ )
#define SO_ARCH_SUFFIX "_amd64.so"
#else
#define SO_ARCH_SUFFIX ".so"
#endif
#endif

#endif

#endif //COMMON_PLATFORM_H