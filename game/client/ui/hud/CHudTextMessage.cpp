/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
// text_message.cpp
//
// implementation of CHudTextMessage class
//
// this class routes messages through titles.txt for localisation
//

#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"

#include "shared/hud/CHudElementRegistry.h"

#include "vgui_TeamFortressViewport.h"

#include "ui/shared/CLocalize.h"

#include "CHudSayText.h"
#include "CHudTextMessage.h"

DECLARE_MESSAGE( CHudTextMessage, TextMsg );

REGISTER_HUDELEMENT( CHudTextMessage, 40 );

CHudTextMessage::CHudTextMessage( const char* const pszName )
	: BaseClass( pszName )
{
}

bool CHudTextMessage::Init()
{
	HOOK_MESSAGE( TextMsg );

	Reset();

	return true;
}

//TODO: move these to stringutils - Solokiller
void StripEndNewlineFromString( char *str )
{
	int s = strlen( str ) - 1;
	if ( str[s] == '\n' || str[s] == '\r' )
		str[s] = 0;
}

// converts all '\r' characters to '\n', so that the engine can deal with the properly
// returns a pointer to str
char* ConvertCRtoNL( char *str )
{
	for ( char *ch = str; *ch != 0; ch++ )
		if ( *ch == '\r' )
			*ch = '\n';
	return str;
}

// Message handler for text messages
// displays a string, looking them up from the titles.txt file, which can be localised
// parameters:
//   byte:   message direction  ( HUD_PRINTCONSOLE, HUD_PRINTNOTIFY, HUD_PRINTCENTER, HUD_PRINTTALK )
//   string: message
// optional parameters:
//   string: message parameter 1
//   string: message parameter 2
//   string: message parameter 3
//   string: message parameter 4
// any string that starts with the character '#' is a message name, and is used to look up the real message in titles.txt
// the next (optional) one to four strings are parameters for that string (which can also be message names if they begin with '#')
int CHudTextMessage::MsgFunc_TextMsg( const char *pszName, int iSize, void *pbuf )
{
	CBufferReader reader( pbuf, iSize );

	int msg_dest = reader.ReadByte();

#define MSG_BUF_SIZE 128
	static char szBuf[6][MSG_BUF_SIZE];
	char* msg_text = safe_strcpy( szBuf[0], Localize().LookupString( reader.ReadString(), &msg_dest ), MSG_BUF_SIZE);

	// keep reading strings and using C format strings for subsituting the strings into the localised text string
	char* sstr1 = safe_strcpy( szBuf[1], Localize().LookupString( reader.ReadString() ), MSG_BUF_SIZE);
	StripEndNewlineFromString( sstr1 );  // these strings are meant for subsitution into the main strings, so cull the automatic end newlines
	char* sstr2 = safe_strcpy( szBuf[2], Localize().LookupString( reader.ReadString() ), MSG_BUF_SIZE);
	StripEndNewlineFromString( sstr2 );
	char* sstr3 = safe_strcpy( szBuf[3], Localize().LookupString( reader.ReadString() ), MSG_BUF_SIZE);
	StripEndNewlineFromString( sstr3 );
	char* sstr4 = safe_strcpy( szBuf[4], Localize().LookupString( reader.ReadString() ), MSG_BUF_SIZE);
	StripEndNewlineFromString( sstr4 );
	char *psz = szBuf[5];

	if ( gViewPort && !gViewPort->AllowedToPrintText() )
		return 1;

	switch ( msg_dest )
	{
	case HUD_PRINTCENTER:
		safe_sprintf( psz, MSG_BUF_SIZE, msg_text, sstr1, sstr2, sstr3, sstr4 );
		CenterPrint( ConvertCRtoNL( psz ) );
		break;

	case HUD_PRINTNOTIFY:
		psz[0] = 1;  // mark this message to go into the notify buffer
		safe_sprintf( psz+1, MSG_BUF_SIZE, msg_text, sstr1, sstr2, sstr3, sstr4 );
		ConsolePrint( ConvertCRtoNL( psz ) );
		break;

	case HUD_PRINTTALK:
		if( auto pSayText = GETHUDCLASS( CHudSayText ) )
		{
			safe_sprintf( psz, MSG_BUF_SIZE, msg_text, sstr1, sstr2, sstr3, sstr4 );
			pSayText->SayTextPrint( ConvertCRtoNL( psz ), 128 );
		}
		break;

	case HUD_PRINTCONSOLE:
		safe_sprintf( psz, MSG_BUF_SIZE, msg_text, sstr1, sstr2, sstr3, sstr4 );
		ConsolePrint( ConvertCRtoNL( psz ) );
		break;
	}

	return 1;
}
