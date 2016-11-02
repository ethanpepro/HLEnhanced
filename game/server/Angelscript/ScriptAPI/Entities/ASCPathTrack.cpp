#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "entities/plats/CPathTrack.h"

#include "ASCPathTrack.h"

void CPathTrack_Project( CPathTrack* pThis, CPathTrack* pStart, CPathTrack* pEnd, const Vector& vecStartOrigin, Vector& vecOutOrigin, float dist )
{
	vecOutOrigin = vecStartOrigin;

	pThis->Project( pStart, pEnd, vecOutOrigin, dist );
}

CPathTrack* CPathTrack_LookAhead( CPathTrack* pThis, const Vector& vecStartOrigin, Vector& origin, float dist, const bool bMove )
{
	origin = vecStartOrigin;

	return pThis->LookAhead( origin, dist, bMove );
}
