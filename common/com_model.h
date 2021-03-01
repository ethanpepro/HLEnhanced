//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// com_model.h
#if !defined( COM_MODEL_H )
#define COM_MODEL_H
#if defined( _WIN32 )
#pragma once
#endif

struct efrag_t;
struct mleaf_t;
struct msurface_t;
struct surfcache_t;

#define STUDIO_RENDER 1
#define STUDIO_EVENTS 2

#define MAX_CLIENTS			32
#define	MAX_EDICTS			900

#define MAX_MODEL_NAME		64
#define MAX_MAP_HULLS		4
#define	MIPLEVELS			4
#define	NUM_AMBIENTS		4		// automatic ambient sounds
#define	MAXLIGHTMAPS		4
#define	PLANE_ANYZ			5

#define ALIAS_Z_CLIP_PLANE	5

// flags in finalvert_t.flags
#define ALIAS_LEFT_CLIP				0x0001
#define ALIAS_TOP_CLIP				0x0002
#define ALIAS_RIGHT_CLIP			0x0004
#define ALIAS_BOTTOM_CLIP			0x0008
#define ALIAS_Z_CLIP				0x0010
#define ALIAS_ONSEAM				0x0020
#define ALIAS_XY_CLIP_MASK			0x000F

#define	ZISCALE	((float)0x8000)

#define CACHE_SIZE	32		// used to align key data structures

enum modtype_t
{
	mod_brush, 
	mod_sprite, 
	mod_alias, 
	mod_studio
};

// must match definition in modelgen.h
#ifndef SYNCTYPE_T
#define SYNCTYPE_T

enum synctype_t
{
	ST_SYNC=0,
	ST_RAND
};

#endif

struct dmodel_t
{
	Vector		mins, maxs;
	Vector		origin;
	int			headnode[MAX_MAP_HULLS];
	int			visleafs;		// not including the solid leaf 0
	int			firstface, numfaces;
};

// plane_t structure
struct mplane_t
{
	Vector	normal;			// surface normal
	float	dist;			// closest appoach to origin
	byte	type;			// for texture axis selection and fast side tests
	byte	signbits;		// signx + signy<<1 + signz<<1
	byte	pad[2];
};

struct mvertex_t
{
	Vector		position;
};

#define SURF_PLANEBACK 2
#define SURF_DRAWSKY 4
#define SURF_DRAWSPRITE 8
#define SURF_DRAWTURB 0x10
#define SURF_DRAWTILED 0x20
#define SURF_DRAWBACKGROUND 0x40
#define SURF_UNDERWATER 0x80

struct medge_t
{
	unsigned short	v[2];
	unsigned int	cachededgeoffset;
};

#if 0
struct texture_t
{
	char		name[16];
	unsigned	width, height;
	int			anim_total;				// total tenths in sequence ( 0 = no)
	int			anim_min, anim_max;		// time for this frame min <=time< max
	texture_t	*anim_next;		// in the animation sequence
	texture_t	*alternate_anims;	// bmodels in frame 1 use these
	unsigned	offsets[MIPLEVELS];		// four mip maps stored
	unsigned	paloffset;
};
#else
struct texture_t {
	char name[16];
	unsigned width;
	unsigned height;
	int gl_texturenum;
	msurface_t *texturechain;
	int anim_total;
	int anim_min;
	int anim_max;
	texture_t *anim_next;
	texture_t *alternate_anims;
	unsigned offsets[MIPLEVELS];
};
#endif

struct mtexinfo_t
{
	float		vecs[2][4];		// [s/t] unit vectors in world space. 
								// [i][3] is the s/t offset relative to the origin.
								// s or t = dot(3Dpoint,vecs[i])+vecs[i][3]
	float		mipadjust;		// ?? mipmap limits for very small surfaces
	texture_t	*texture;
	int			flags;			// sky or slime, no lightmap or 256 subdivision
};

#if 0

#else
#define VERTEXSIZE 7

struct glpoly_t {
	glpoly_t *next;
	glpoly_t *chain;
	int numverts;
	int flags;
	float verts[4][VERTEXSIZE];
};
#endif

#if 0
struct mnode_t
{
// common with leaf
	int			contents;		// 0, to differentiate from leafs
	int			visframe;		// node needs to be traversed if current
	
	short		minmaxs[6];		// for bounding box culling

	mnode_t		*parent;

// node specific
	mplane_t	*plane;
	mnode_t		*children[2];

	unsigned short		firstsurface;
	unsigned short		numsurfaces;
};
#else
struct mnode_t {
	int contents;
	int visframe;
#if 0
	float minmaxs[6];
#else
	Vector mins;
	Vector maxs;
#endif
	mnode_t *parent;
	mplane_t *plane;
	mnode_t *children[2];
	unsigned short firstsurface;
	unsigned short numsurfaces;
};
#endif

// JAY: Compress this as much as possible
struct decal_t
{
	decal_t		*pnext;			// linked list for each surface
	msurface_t	*psurface;		// Surface id for persistence / unlinking
	short		dx;				// Offsets into surface texture (in texture coordinates, so we don't need floats)
	short		dy;
	short		texture;		// Decal texture
	byte		scale;			// Pixel scale
	byte		flags;			// Decal flags

	short		entityIndex;	// Entity this is attached to
};

#if 0
struct mleaf_t
{
// common with node
	int			contents;		// wil be a negative contents number
	int			visframe;		// node needs to be traversed if current

	short		minmaxs[6];		// for bounding box culling

	mnode_t		*parent;

// leaf specific
	byte		*compressed_vis;
	efrag_t		*efrags;

	msurface_t	**firstmarksurface;
	int			nummarksurfaces;
	int			key;			// BSP sequence number for leaf's contents
	byte		ambient_sound_level[NUM_AMBIENTS];
};
#else
struct mleaf_t {
	int contents;
	int visframe;
#if 0
	float minmaxs[6];
#else
	Vector mins;
	Vector maxs;
#endif
	mnode_t *parent;
	byte *compressed_vis;
	efrag_t *efrags;
	msurface_t **firstmarksurface;
	int nummarksurfaces;
	int key;
	byte ambient_sound_level[NUM_AMBIENTS];
};
#endif

#if 0
struct msurface_t
{
	int			visframe;		// should be drawn when node is crossed

	int			dlightframe;	// last frame the surface was checked by an animated light
	int			dlightbits;		// dynamically generated. Indicates if the surface illumination 
								// is modified by an animated light.

	mplane_t	*plane;			// pointer to shared plane			
	int			flags;			// see SURF_ #defines

	int			firstedge;	// look up in model->surfedges[], negative numbers
	int			numedges;	// are backwards edges
	
// surface generation data
	surfcache_t	*cachespots[MIPLEVELS];

	short		texturemins[2]; // smallest s/t position on the surface.
	short		extents[2];		// ?? s/t texture size, 1..256 for all non-sky surfaces

	mtexinfo_t	*texinfo;		
	
// lighting info
	byte		styles[MAXLIGHTMAPS]; // index into d_lightstylevalue[] for animated lights 
									  // no one surface can be effected by more than 4 
									  // animated lights.
	color24		*samples;
	
	decal_t		*pdecals;
};
#else
struct msurface_t {
	int visframe;
	mplane_t *plane;
	int flags;
	int firstedge;
	int numedges;
	short texturemins[2];
	short extents[2];
	int light_s;
	int light_t;
	glpoly_t *polys;
	msurface_t *texturechain;
	mtexinfo_t *texinfo;
	int dlightframe;
	int dlightbits;
	int lightmaptexturenum;
	byte styles[MAXLIGHTMAPS];
	int cached_light[MAXLIGHTMAPS];
	qboolean cached_dlight;
	color24 *samples;
	decal_t *pdecals;
};
#endif

struct dclipnode_t
{
	int			planenum;
	short		children[2];	// negative numbers are contents
};

struct hull_t
{
	dclipnode_t	*clipnodes;
	mplane_t	*planes;
	int			firstclipnode;
	int			lastclipnode;
	Vector		clip_mins;
	Vector		clip_maxs;
};

#if !defined( CACHE_USER ) && !defined( QUAKEDEF_H )
#define CACHE_USER
struct cache_user_t
{
	void	*data;
};
#endif

struct model_t
{
	char		name[ MAX_MODEL_NAME ];
	qboolean	needload;		// bmodels and sprites don't cache normally

	modtype_t	type;
	int			numframes;
	synctype_t	synctype;
	
	int			flags;

//
// volume occupied by the model
//		
	Vector		mins, maxs;
	float		radius;

//
// brush model
//
	int			firstmodelsurface, nummodelsurfaces;

	int			numsubmodels;
	dmodel_t	*submodels;

	int			numplanes;
	mplane_t	*planes;

	int			numleafs;		// number of visible leafs, not counting 0
	mleaf_t		*leafs;

	int			numvertexes;
	mvertex_t	*vertexes;

	int			numedges;
	medge_t		*edges;

	int			numnodes;
	mnode_t		*nodes;

	int			numtexinfo;
	mtexinfo_t	*texinfo;

	int			numsurfaces;
	msurface_t	*surfaces;

	int			numsurfedges;
	int			*surfedges;

	int			numclipnodes;
	dclipnode_t	*clipnodes;

	int			nummarksurfaces;
	msurface_t	**marksurfaces;

	hull_t		hulls[MAX_MAP_HULLS];

	int			numtextures;
	texture_t	**textures;

	byte		*visdata;

	color24		*lightdata;

	char		*entities;

//
// additional model data
//
	cache_user_t	cache;		// only access through Mod_Extradata

};

struct alight_t
{
	int			ambientlight;	// clip at 128
	int			shadelight;		// clip at 192 - ambientlight
	Vector		color;
	float		*plightvec;
};

struct auxvert_t
{
	Vector	fv;		// viewspace x, y
};

#include "custom.h"

#define	MAX_INFO_STRING			256
#define	MAX_SCOREBOARDNAME		32
struct player_info_t
{
	// User id on server
	int		userid;

	// User info string
	char	userinfo[ MAX_INFO_STRING ];

	// Name
	char	name[ MAX_SCOREBOARDNAME ];

	// Spectator or not, unused
	int		spectator;

	int		ping;
	int		packet_loss;

	// skin information
	char	model[MAX_QPATH];
	int		topcolor;
	int		bottomcolor;

	// last frame rendered
	int		renderframe;	

	// Gait frame estimation
	int		gaitsequence;
	float	gaitframe;
	float	gaityaw;
	Vector	prevgaitorigin;

	customization_t customdata;
};

#endif // #define COM_MODEL_H
