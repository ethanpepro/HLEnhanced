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
#if USE_OPFOR
#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "CDEagleLaser.h"

LINK_ENTITY_TO_CLASS( eagle_laser, CDEagleLaser );

//=========================================================
//=========================================================
CDEagleLaser *CDEagleLaser::CreateSpot()
{
	CDEagleLaser *pSpot = GetClassPtr( ( CDEagleLaser* ) NULL );
	pSpot->Spawn();

	pSpot->pev->classname = MAKE_STRING( "eagle_laser" );

	return pSpot;
}

#endif //USE_OPFOR
