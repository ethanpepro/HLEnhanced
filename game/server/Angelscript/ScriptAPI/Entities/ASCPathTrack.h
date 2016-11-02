#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCPATHTRACK_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCPATHTRACK_H

#include "Angelscript/ScriptAPI/Entities/ASCBaseEntity.h"

void CPathTrack_Project( CPathTrack* pThis, CPathTrack* pStart, CPathTrack* pEnd, const Vector& vecStartOrigin, Vector& vecOutOrigin, float dist );

CPathTrack* CPathTrack_LookAhead( CPathTrack* pThis, const Vector& vecStartOrigin, Vector& origin, float dist, const bool bMove );

#define AS_CPATHTRACK_NAME "CPathTrack"

template<typename CLASS>
void RegisterScriptCPathTrack( asIScriptEngine& engine, const char* const pszObjectName )
{
	RegisterScriptCBaseEntity<CLASS>( engine, pszObjectName );

	as::RegisterCasts<CPathTrack, CLASS>( engine, AS_CPATHTRACK_NAME, pszObjectName, &as::Cast_UpCast, &as::Cast_DownCast );

	engine.RegisterObjectMethod(
		pszObjectName, "void SetPrevious(CPathTrack@ pPrevious)",
		asMETHOD( CLASS, SetPrevious ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Link()",
		asMETHOD( CLASS, Link ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CPathTrack@ ValidPath(CPathTrack@ pPrevious, const bool bTestFlag)",
		asMETHOD( CLASS, ValidPath ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Project(CPathTrack@ pStart, CPathTrack@ pEnd, const Vector& in vecStartOrigin, Vector& out vecOutOrigin, float dist)",
		asFUNCTION( CPathTrack_Project ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "CPathTrack@ LookAhead(const Vector& in vecStartOrigin, Vector& out vecOutOrigin, float dist, const bool bMove)",
		asFUNCTION( CPathTrack_LookAhead ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "CPathTrack@ Nearest(Vector origin)",
		asMETHOD( CLASS, Nearest ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CPathTrack@ GetNext()",
		asMETHOD( CLASS, GetNext ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CPathTrack@ GetPrevious()",
		asMETHOD( CLASS, GetPrevious ), asCALL_THISCALL );

	engine.RegisterObjectProperty(
		pszObjectName, "float m_length", 
		asOFFSET( CLASS, m_length ) );

	engine.RegisterObjectProperty(
		pszObjectName, "string_t m_altName",
		asOFFSET( CLASS, m_altName ) );

	engine.RegisterObjectProperty(
		pszObjectName, "CPathTrack@ m_pnext",
		asOFFSET( CLASS, m_pnext ) );

	engine.RegisterObjectProperty(
		pszObjectName, "CPathTrack@ m_pprevious",
		asOFFSET( CLASS, m_pprevious ) );

	engine.RegisterObjectProperty(
		pszObjectName, "CPathTrack@ m_paltpath",
		asOFFSET( CLASS, m_paltpath ) );
}

inline void RegisterScriptCPathTrack( asIScriptEngine& engine )
{
	RegisterScriptCPathTrack<CPathTrack>( engine, AS_CPATHTRACK_NAME );
}

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_ENTITIES_ASCPATHTRACK_H
