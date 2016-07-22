#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CRenderFxManager.h"

LINK_ENTITY_TO_CLASS( env_render, CRenderFxManager );

void CRenderFxManager::Spawn( void )
{
	pev->solid = SOLID_NOT;
}

void CRenderFxManager::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if( HasTarget() )
	{
		edict_t* pentTarget = NULL;
		while( 1 )
		{
			pentTarget = FIND_ENTITY_BY_TARGETNAME( pentTarget, GetTarget() );
			if( FNullEnt( pentTarget ) )
				break;

			entvars_t *pevTarget = VARS( pentTarget );
			if( !FBitSet( pev->spawnflags, SF_RENDER_MASKFX ) )
				pevTarget->renderfx = pev->renderfx;
			if( !FBitSet( pev->spawnflags, SF_RENDER_MASKAMT ) )
				pevTarget->renderamt = pev->renderamt;
			if( !FBitSet( pev->spawnflags, SF_RENDER_MASKMODE ) )
				pevTarget->rendermode = pev->rendermode;
			if( !FBitSet( pev->spawnflags, SF_RENDER_MASKCOLOR ) )
				pevTarget->rendercolor = pev->rendercolor;
		}
	}
}