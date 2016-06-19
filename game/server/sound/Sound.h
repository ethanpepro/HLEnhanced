/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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
#ifndef GAME_SERVER_SOUND_SOUND_H
#define GAME_SERVER_SOUND_SOUND_H

// sentence groups
#define CBSENTENCENAME_MAX 16
#define CSENTENCE_LRU_MAX	32		// max number of elements per sentence group
#define CVOXFILESENTENCEMAX		1536		// max number of sentences in game. NOTE: this must match
// CVOXFILESENTENCEMAX in engine\sound.h!!!

struct SENTENCEG
{
	char szgroupname[ CBSENTENCENAME_MAX ];
	int count;
	unsigned char rgblru[ CSENTENCE_LRU_MAX ];
};

extern char gszallsentencenames[ CVOXFILESENTENCEMAX ][ CBSENTENCENAME_MAX ];
extern int gcallsentences;

int USENTENCEG_Pick( int isentenceg, char *szfound );
int USENTENCEG_PickSequential( int isentenceg, char *szfound, int ipick, const bool bReset );
void USENTENCEG_InitLRU( unsigned char *plru, int count );

void SENTENCEG_Init();
void SENTENCEG_Stop( edict_t *entity, int isentenceg, int ipick );
int SENTENCEG_PlayRndI( edict_t *entity, int isentenceg, float volume, float attenuation, int flags, int pitch );
int SENTENCEG_PlayRndSz( edict_t *entity, const char *szrootname, float volume, float attenuation, int flags, int pitch );
int SENTENCEG_PlaySequentialSz( edict_t *entity, const char *szrootname, float volume, float attenuation, int flags, int pitch, int ipick, const bool bReset );
int SENTENCEG_GetIndex( const char *szrootname );
int SENTENCEG_Lookup( const char *sample, char *sentencenum );

void TEXTURETYPE_Init();
char TEXTURETYPE_Find( char *name );
float TEXTURETYPE_PlaySound( TraceResult *ptr, Vector vecSrc, Vector vecEnd, int iBulletType );

// NOTE: use EMIT_SOUND_DYN to set the pitch of a sound. Pitch of 100
// is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
// down to 1 is a lower pitch.   150 to 70 is the realistic range.
// EMIT_SOUND_DYN with pitch != 100 should be used sparingly, as it's not quite as
// fast as EMIT_SOUND (the pitchshift mixer is not native coded).

void EMIT_SOUND_DYN( edict_t *entity, int channel, const char *sample, float volume, float attenuation,
					 int flags, int pitch );


inline void EMIT_SOUND( edict_t *entity, int channel, const char *sample, float volume, float attenuation )
{
	EMIT_SOUND_DYN( entity, channel, sample, volume, attenuation, 0, PITCH_NORM );
}

inline void STOP_SOUND( edict_t *entity, int channel, const char *sample )
{
	EMIT_SOUND_DYN( entity, channel, sample, 0, 0, SND_STOP, PITCH_NORM );
}

void EMIT_SOUND_SUIT( edict_t *entity, const char *sample );
void EMIT_GROUPID_SUIT( edict_t *entity, int isentenceg );
void EMIT_GROUPNAME_SUIT( edict_t *entity, const char *groupname );

#endif //GAME_SERVER_SOUND_SOUND_H