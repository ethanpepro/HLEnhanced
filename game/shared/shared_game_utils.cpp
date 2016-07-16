#include <cassert>
#include <cstdarg>

#include "extdll.h"
#include "util.h"

#ifdef CLIENT_DLL
//TODO: make headers compatible for both dlls - Solokiller
#undef CVAR_GET_FLOAT
#undef CVAR_GET_STRING
#include "hud.h"
#include "cl_util.h"
#endif

#include "cbase.h"

#include "shared_game_utils.h"

DLL_GLOBAL const Vector	g_vecZero = Vector( 0, 0, 0 );

cvar_t* g_pDeveloper = nullptr;

CBaseEntity* GET_PRIVATE( edict_t* pent )
{
	if( pent )
		return reinterpret_cast<CBaseEntity*>( pent->pvPrivateData );

	return nullptr;
}

CHashStringPool g_StringPool;

bool FNullEnt( const CBaseEntity* pEntity )
{
	return pEntity == nullptr || FNullEnt( pEntity->pev );
}

static unsigned int glSeed = 0;

unsigned int seed_table[ 256 ] =
{
	28985, 27138, 26457, 9451, 17764, 10909, 28790, 8716, 6361, 4853, 17798, 21977, 19643, 20662, 10834, 20103,
	27067, 28634, 18623, 25849, 8576, 26234, 23887, 18228, 32587, 4836, 3306, 1811, 3035, 24559, 18399, 315,
	26766, 907, 24102, 12370, 9674, 2972, 10472, 16492, 22683, 11529, 27968, 30406, 13213, 2319, 23620, 16823,
	10013, 23772, 21567, 1251, 19579, 20313, 18241, 30130, 8402, 20807, 27354, 7169, 21211, 17293, 5410, 19223,
	10255, 22480, 27388, 9946, 15628, 24389, 17308, 2370, 9530, 31683, 25927, 23567, 11694, 26397, 32602, 15031,
	18255, 17582, 1422, 28835, 23607, 12597, 20602, 10138, 5212, 1252, 10074, 23166, 19823, 31667, 5902, 24630,
	18948, 14330, 14950, 8939, 23540, 21311, 22428, 22391, 3583, 29004, 30498, 18714, 4278, 2437, 22430, 3439,
	28313, 23161, 25396, 13471, 19324, 15287, 2563, 18901, 13103, 16867, 9714, 14322, 15197, 26889, 19372, 26241,
	31925, 14640, 11497, 8941, 10056, 6451, 28656, 10737, 13874, 17356, 8281, 25937, 1661, 4850, 7448, 12744,
	21826, 5477, 10167, 16705, 26897, 8839, 30947, 27978, 27283, 24685, 32298, 3525, 12398, 28726, 9475, 10208,
	617, 13467, 22287, 2376, 6097, 26312, 2974, 9114, 21787, 28010, 4725, 15387, 3274, 10762, 31695, 17320,
	18324, 12441, 16801, 27376, 22464, 7500, 5666, 18144, 15314, 31914, 31627, 6495, 5226, 31203, 2331, 4668,
	12650, 18275, 351, 7268, 31319, 30119, 7600, 2905, 13826, 11343, 13053, 15583, 30055, 31093, 5067, 761,
	9685, 11070, 21369, 27155, 3663, 26542, 20169, 12161, 15411, 30401, 7580, 31784, 8985, 29367, 20989, 14203,
	29694, 21167, 10337, 1706, 28578, 887, 3373, 19477, 14382, 675, 7033, 15111, 26138, 12252, 30996, 21409,
	25678, 18555, 13256, 23316, 22407, 16727, 991, 9236, 5373, 29402, 6117, 15241, 27715, 19291, 19888, 19847
};

unsigned int U_Random( void )
{
	glSeed *= 69069;
	glSeed += seed_table[ glSeed & 0xff ];

	return ( ++glSeed & 0x0fffffff );
}

void U_Srand( unsigned int seed )
{
	glSeed = seed_table[ seed & 0xff ];
}

/*
=====================
UTIL_SharedRandomLong
=====================
*/
int UTIL_SharedRandomLong( unsigned int seed, int low, int high )
{
	unsigned int range;

	U_Srand( ( int ) seed + low + high );

	range = high - low + 1;
	if( !( range - 1 ) )
	{
		return low;
	}
	else
	{
		int offset;
		int rnum;

		rnum = U_Random();

		offset = rnum % range;

		return ( low + offset );
	}
}

/*
=====================
UTIL_SharedRandomFloat
=====================
*/
float UTIL_SharedRandomFloat( unsigned int seed, float low, float high )
{
	//
	unsigned int range;

	U_Srand( ( int ) seed + *( int * ) &low + *( int * ) &high );

	U_Random();
	U_Random();

	range = high - low;
	if( !range )
	{
		return low;
	}
	else
	{
		int tensixrand;
		float offset;

		tensixrand = U_Random() & 65535;

		offset = ( float ) tensixrand / 65536.0;

		return ( low + offset * range );
	}
}

void UTIL_StringToVector( float *pVector, const char *pString )
{
	char *pstr, *pfront, tempString[ 128 ];
	int	j;

	strcpy( tempString, pString );
	pstr = pfront = tempString;

	for( j = 0; j < 3; j++ )			// lifted from pr_edict.c
	{
		pVector[ j ] = atof( pfront );

		while( *pstr && *pstr != ' ' )
			pstr++;
		if( !*pstr )
			break;
		pstr++;
		pfront = pstr;
	}
	if( j < 2 )
	{
		/*
		ALERT( at_error, "Bad field in entity!! %s:%s == \"%s\"\n",
		pkvd->szClassName, pkvd->szKeyName, pkvd->szValue );
		*/
		for( j = j + 1; j < 3; j++ )
			pVector[ j ] = 0;
	}
}

void UTIL_StringToIntArray( int *pVector, int count, const char *pString )
{
	char *pstr, *pfront, tempString[ 128 ];
	int	j;

	strcpy( tempString, pString );
	pstr = pfront = tempString;

	for( j = 0; j < count; j++ )			// lifted from pr_edict.c
	{
		pVector[ j ] = atoi( pfront );

		while( *pstr && *pstr != ' ' )
			pstr++;
		if( !*pstr )
			break;
		pstr++;
		pfront = pstr;
	}

	for( j++; j < count; j++ )
	{
		pVector[ j ] = 0;
	}
}

//TODO: add support for multiple buffers - Solokiller
char* UTIL_VarArgs( const char* pszFormat, ... )
{
	va_list argptr;
	static char string[ 1024 ];

	va_start( argptr, pszFormat );
	vsprintf( string, pszFormat, argptr );
	va_end( argptr );

	return string;
}

char *UTIL_dtos1( int d )
{
	//TODO: use larger buffer, multiple buffers - Solokliler
	static char buf[ 8 ];
	sprintf( buf, "%d", d );
	return buf;
}

char *UTIL_dtos2( int d )
{
	static char buf[ 8 ];
	sprintf( buf, "%d", d );
	return buf;
}

char *UTIL_dtos3( int d )
{
	static char buf[ 8 ];
	sprintf( buf, "%d", d );
	return buf;
}

char *UTIL_dtos4( int d )
{
	static char buf[ 8 ];
	sprintf( buf, "%d", d );
	return buf;
}

//=========================================================
// UTIL_StripToken - for redundant keynames
//=========================================================
void UTIL_StripToken( const char *pKey, char *pDest )
{
	int i = 0;

	while( pKey[ i ] && pKey[ i ] != '#' )
	{
		pDest[ i ] = pKey[ i ];
		i++;
	}
	pDest[ i ] = 0;
}

char *safe_strcpy( char *dst, const char *src, const size_t len_dst )
{
	if( len_dst <= 0 )
	{
		return nullptr; // this is bad
	}

	strncpy( dst, src, len_dst );
	dst[ len_dst - 1 ] = '\0';

	return dst;
}

int safe_sprintf( char *dst, const size_t len_dst, const char *format, ... )
{
	if( len_dst <= 0 )
	{
		return -1; // this is bad
	}

	va_list v;

	va_start( v, format );

	_vsnprintf( dst, len_dst, format, v );

	va_end( v );

	dst[ len_dst - 1 ] = '\0';

	return 0;
}

#ifdef	DEBUG
void
DBG_AssertFunction(
	const bool	fExpr,
	const char*	szExpr,
	const char*	szFile,
	int			szLine,
	const char*	szMessage )
{
	if( fExpr )
		return;
	char szOut[ 512 ];
	if( szMessage != NULL )
		sprintf( szOut, "ASSERT FAILED:\n %s \n(%s@%d)\n%s", szExpr, szFile, szLine, szMessage );
	else
		sprintf( szOut, "ASSERT FAILED:\n %s \n(%s@%d)", szExpr, szFile, szLine );
	ALERT( at_console, szOut );
}
#endif	// DEBUG

char com_token[ MAX_COM_TOKEN ];

/*
==============
COM_Parse

Parse a token out of a string
==============
*/
char *COM_Parse( char *data )
{
	int             c;
	int             len;

	len = 0;
	com_token[ 0 ] = 0;

	if( !data )
		return NULL;

	// skip whitespace
skipwhite:
	while( ( c = *data ) <= ' ' )
	{
		if( c == 0 )
			return NULL;                    // end of file;
		data++;
	}

	// skip // comments
	if( c == '/' && data[ 1 ] == '/' )
	{
		while( *data && *data != '\n' )
			data++;
		goto skipwhite;
	}


	// handle quoted strings specially
	if( c == '\"' )
	{
		data++;
		while( 1 )
		{
			c = *data++;
			if( c == '\"' || !c )
			{
				com_token[ len ] = 0;
				return data;
			}
			com_token[ len ] = c;
			len++;
		}
	}

	// parse single characters
	if( c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ',' )
	{
		com_token[ len ] = c;
		len++;
		com_token[ len ] = 0;
		return data + 1;
	}

	// parse a regular word
	do
	{
		com_token[ len ] = c;
		data++;
		len++;
		c = *data;
		if( c == '{' || c == '}' || c == ')' || c == '(' || c == '\'' || c == ',' )
			break;
	}
	while( c>32 );

	com_token[ len ] = 0;
	return data;
}

/*
==============
COM_TokenWaiting

Returns 1 if additional data is waiting to be processed on this line
==============
*/
int COM_TokenWaiting( char *buffer )
{
	char *p;

	p = buffer;
	while( *p && *p != '\n' )
	{
		if( !isspace( *p ) || isalnum( *p ) )
			return 1;

		p++;
	}

	return 0;
}

char *memfgets( byte *pMemFile, int fileSize, int &filePos, char *pBuffer, int bufferSize )
{
	// Bullet-proofing
	if( !pMemFile || !pBuffer )
		return NULL;

	if( filePos >= fileSize )
		return NULL;

	int i = filePos;
	int last = fileSize;

	// fgets always NULL terminates, so only read bufferSize-1 characters
	if( last - filePos > ( bufferSize - 1 ) )
		last = filePos + ( bufferSize - 1 );

	int stop = 0;

	// Stop at the next newline (inclusive) or end of buffer
	while( i < last && !stop )
	{
		if( pMemFile[ i ] == '\n' )
			stop = 1;
		i++;
	}


	// If we actually advanced the pointer, copy it over
	if( i != filePos )
	{
		// We read in size bytes
		int size = i - filePos;
		// copy it out
		memcpy( pBuffer, pMemFile + filePos, sizeof( byte )*size );

		// If the buffer isn't full, terminate (this is always true)
		if( size < bufferSize )
			pBuffer[ size ] = 0;

		// Update file pointer
		filePos = i;
		return pBuffer;
	}

	// No data read, bail
	return NULL;
}

cvar_t* CVarGetPointer( const char* const pszName )
{
	return
#ifndef CLIENT_DLL
	CVAR_GET_POINTER( pszName )
#else
	gEngfuncs.pfnGetCvarPointer( pszName )
#endif
		;
}

void Alert( int aType, const char* const pszFormat, ... )
{
	assert( pszFormat );

	va_list list;

	va_start( list, pszFormat );

	char szBuffer[ 4096 ];

	const int iResult = vsnprintf( szBuffer, sizeof( szBuffer ), pszFormat, list );

	if( iResult >= 0 && static_cast<size_t>( iResult ) < sizeof( szBuffer ) )
	{
#ifndef CLIENT_DLL
		ALERT( static_cast<ALERT_TYPE>( aType ), "%s", szBuffer );
#else
		//Mimic the server version.
		if( g_pDeveloper->value != 0 || gEngfuncs.GetMaxClients() <= 1 )
		{
			const char* pszPrefix = nullptr;

			bool bContinue = true;

			switch( aType )
			{
			case at_notice:
				{
					pszPrefix = "NOTE: ";
					break;
				}

			case at_aiconsole:
				{
					if( g_pDeveloper->value < 2 )
						bContinue = false;
					break;
				}

			case at_warning:
				{
					pszPrefix = "WARNING: ";
					break;
				}

			case at_error:
				{
					pszPrefix = "ERROR: ";
					break;
				}

			case at_logged:
				{
					//No actual logging, just tell the client it was a logged value.
					pszPrefix = "LOGGED: ";
					break;
				}

			case at_console:
			default: break;
			}

			if( bContinue )
			{
				if( pszPrefix )
				{
					gEngfuncs.Con_Printf( "%s%s", pszPrefix, szBuffer );
				}
				else
				{
					gEngfuncs.Con_Printf( "%s", szBuffer );
				}
			}
		}
#endif
	}

	va_end( list );
}