/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// cl_util.h
//
#ifndef GAME_CLIENT_CL_UTIL_H
#define GAME_CLIENT_CL_UTIL_H

#include "cvardef.h"

#include <stdio.h> // for safe_sprintf()
#include <stdarg.h>  // "
#include <string.h> // for strncpy()

#include "shared_game_utils.h"

// Macros to hook function calls into the HUD object
/**
*	Declares a function that calls the HUD class method for the given network message.
*	@param className HUD class name.
*	@param messageName Name of the message. The HUD class method should be named MsgFunc_<messageName>.
*/
#define DECLARE_MESSAGE( className, messageName )							\
int __MsgFunc_##messageName( const char *pszName, int iSize, void *pbuf )	\
{																			\
	return gHUD.className.MsgFunc_##messageName( pszName, iSize, pbuf );	\
}

/**
*	Hooks a network message function.
*	@param messageName Name of the message whose function should be hooked. The function name is __MsgFunc_<messageName>.
*/
#define HOOK_MESSAGE( messageName ) gEngfuncs.pfnHookUserMsg( #messageName, __MsgFunc_##messageName );

/**
*	Declares a function that calls the HUD class method for the given command.
*	@param className HUD class name.
*	@param commandFuncName Name of the command. The HUD class method should be named UserCmd_<commandFuncName>.
*/
#define DECLARE_COMMAND( className, commandFuncName )	\
void __CmdFunc_##commandFuncName()						\
{														\
	gHUD.className.UserCmd_##commandFuncName();			\
}

/**
*	Hooks a command function.
*	@param szCommandName Name of the command whose function should be hooked.
*	@param commandFuncName Name of the command function. The function name is __CmdFunc_<commandFuncName>.
*/
#define HOOK_COMMAND( szCommandName, commandFuncName ) gEngfuncs.pfnAddCommand( szCommandName, __CmdFunc_##commandFuncName );

//TODO: this fixes compilation issues caused by server and client code being in the same file, but isn't ideal. Clean up. - Solokiller
#ifndef CVAR_GET_FLOAT
inline float CVAR_GET_FLOAT( const char *x ) {	return gEngfuncs.pfnGetCvarFloat( (char*)x ); }
inline const char* CVAR_GET_STRING( const char *x ) {	return gEngfuncs.pfnGetCvarString( (char*)x ); }
#endif
inline struct cvar_s *CVAR_CREATE( const char *cv, const char *val, const int flags ) {	return gEngfuncs.pfnRegisterVariable( (char*)cv, (char*)val, flags ); }

#define SPR_Load (*gEngfuncs.pfnSPR_Load)
#define SPR_Set (*gEngfuncs.pfnSPR_Set)
#define SPR_Frames (*gEngfuncs.pfnSPR_Frames)
#define SPR_GetList (*gEngfuncs.pfnSPR_GetList)

// SPR_Draw  draws a the current sprite as solid
#define SPR_Draw (*gEngfuncs.pfnSPR_Draw)
// SPR_DrawHoles  draws the current sprites,  with color index255 not drawn (transparent)
#define SPR_DrawHoles (*gEngfuncs.pfnSPR_DrawHoles)
// SPR_DrawAdditive  adds the sprites RGB values to the background  (additive transulency)
#define SPR_DrawAdditive (*gEngfuncs.pfnSPR_DrawAdditive)

// SPR_EnableScissor  sets a clipping rect for HUD sprites.  (0,0) is the top-left hand corner of the screen.
#define SPR_EnableScissor (*gEngfuncs.pfnSPR_EnableScissor)
// SPR_DisableScissor  disables the clipping rect
#define SPR_DisableScissor (*gEngfuncs.pfnSPR_DisableScissor)

HSPRITE LoadSprite( const char *pszName );

//
#define FillRGBA (*gEngfuncs.pfnFillRGBA)


// ScreenHeight returns the height of the screen, in pixels
#define ScreenHeight (gHUD.m_scrinfo.iHeight)
// ScreenWidth returns the width of the screen, in pixels
#define ScreenWidth (gHUD.m_scrinfo.iWidth)

// use this to project world coordinates to screen coordinates
#define XPROJECT(x)	( (1.0f+(x))*ScreenWidth*0.5f )
#define YPROJECT(y) ( (1.0f-(y))*ScreenHeight*0.5f )

#define XRES(x)					(x  * ((float)ScreenWidth / 640))
#define YRES(y)					(y  * ((float)ScreenHeight / 480))

#define GetScreenInfo (*gEngfuncs.pfnGetScreenInfo)
#define ServerCmd (*gEngfuncs.pfnServerCmd)
#define EngineClientCmd (*gEngfuncs.pfnClientCmd)
#define SetCrosshair (*gEngfuncs.pfnSetCrosshair)

// Gets the height & width of a sprite,  at the specified frame
inline int SPR_Height( HSPRITE x, int f )	{ return gEngfuncs.pfnSPR_Height(x, f); }
inline int SPR_Width( HSPRITE x, int f )	{ return gEngfuncs.pfnSPR_Width(x, f); }

inline 	client_textmessage_t	*TextMessageGet( const char *pName ) { return gEngfuncs.pfnTextMessageGet( pName ); }
inline 	int						TextMessageDrawChar( int x, int y, int number, int r, int g, int b ) 
{ 
	return gEngfuncs.pfnDrawCharacter( x, y, number, r, g, b ); 
}

inline int DrawConsoleString( int x, int y, const char *string )
{
	return gEngfuncs.pfnDrawConsoleString( x, y, (char*) string );
}

inline void GetConsoleStringSize( const char *string, int *width, int *height )
{
	gEngfuncs.pfnDrawConsoleStringLen( string, width, height );
}

inline int ConsoleStringLen( const char *string )
{
	int _width, _height;
	GetConsoleStringSize( string, &_width, &_height );
	return _width;
}

inline void ConsolePrint( const char *string )
{
	gEngfuncs.pfnConsolePrint( string );
}

inline void CenterPrint( const char *string )
{
	gEngfuncs.pfnCenterPrint( string );
}

// sound functions
inline void PlaySound( char *szSound, float vol ) { gEngfuncs.pfnPlaySoundByName( szSound, vol ); }
inline void PlaySound( int iSound, float vol ) { gEngfuncs.pfnPlaySoundByIndex( iSound, vol ); }

#include "MinMax.h"

void ScaleColors( int &r, int &g, int &b, int a );

// disable 'possible loss of data converting float to int' warning message
#pragma warning( disable: 4244 )
// disable 'truncation from 'const double' to 'float' warning message
#pragma warning( disable: 4305 )

inline void UnpackRGB(int &r, int &g, int &b, unsigned long ulRGB)
{
	r = (ulRGB & 0xFF0000) >>16;
	g = (ulRGB & 0xFF00) >> 8;
	b = ulRGB & 0xFF;
}

#endif //GAME_CLIENT_CL_UTIL_H