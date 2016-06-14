#ifndef COMMON_PLATFORM_H
#define COMMON_PLATFORM_H

/*
*	Platform specific code compatibility.
*/

#include <cmath>
#include <cstdlib>
#include <cstring>

#ifdef WIN32
#define stricmp _stricmp
#define strupr _strupr
#define stricmp _stricmp
#define strnicmp _strnicmp

//Define these macros once only. The SDK is such a mess. - Solokiller
#define DLLEXPORT __declspec( dllexport )
#define DLLIMPORT __declspec( dllimport )
#else

#define DLLEXPORT __attribute__( ( visibility( "default" ) ) )
#define DLLIMPORT
#endif

#endif //COMMON_PLATFORM_H