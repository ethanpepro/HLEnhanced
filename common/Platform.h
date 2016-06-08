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
#else
#endif

#endif //COMMON_PLATFORM_H