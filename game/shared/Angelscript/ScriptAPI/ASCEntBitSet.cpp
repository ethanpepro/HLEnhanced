#include "angelscript.h"

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "ASCEntBitSet.h"

static void DefaultConstructCEntBitSet( CEntBitSet* pBitSet )
{
	new( pBitSet ) CEntBitSet();
}

static void ConstructCEntBitSet( CEntBitSet* pBitSet, const int value )
{
	new( pBitSet ) CEntBitSet( value );
}

static void CopyConstructCEntBitSet( CEntBitSet* pBitSet, const CEntBitSet& other )
{
	new( pBitSet ) CEntBitSet( other );
}

static void DestructCEntBitSet( CEntBitSet* pBitSet )
{
	pBitSet->~CEntBitSet();
}

void RegisterScriptCEntBitSet( asIScriptEngine& engine )
{
	const char* const pszObjectName = "CEntBitSet";

	engine.RegisterObjectType( pszObjectName, sizeof( CEntBitSet ), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void CEntBitSet()",
		asFUNCTION( DefaultConstructCEntBitSet ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour( 
		pszObjectName, asBEHAVE_CONSTRUCT, "void CEntBitSet(const int value)", 
		asFUNCTION( ConstructCEntBitSet ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void CEntBitSet(const CEntBitSet& in other)",
		asFUNCTION( CopyConstructCEntBitSet ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_DESTRUCT, "void CEntBitSet()",
		asFUNCTION( DestructCEntBitSet ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& opAssign(const CEntBitSet& in other)", 
		asMETHODPR( CEntBitSet, operator=, ( const CEntBitSet& ), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& opAssign(const int value)",
		asMETHODPR( CEntBitSet, operator=, ( const int ), CEntBitSet& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool Any(const int flags) const",
		asMETHOD( CEntBitSet, Any ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool All(const int flags) const",
		asMETHOD( CEntBitSet, All ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int Get() const",
		asMETHOD( CEntBitSet, Get ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void Set(const int flags)",
		asMETHOD( CEntBitSet, Set ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void AddFlags(const int flags)",
		asMETHOD( CEntBitSet, AddFlags ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearAll()",
		asMETHOD( CEntBitSet, ClearAll ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "void ClearFlags(const int flags)",
		asMETHOD( CEntBitSet, ClearFlags ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int opImplConv() const",
		asMETHOD( CEntBitSet, operator int ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& opOrAssign(const int flags)",
		asMETHOD( CEntBitSet, operator|= ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& opAndAssign(const int flags)",
		asMETHOD( CEntBitSet, operator&= ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet& opXorAssign(const int flags)",
		asMETHOD( CEntBitSet, operator^= ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet opOr(const int flags)",
		asMETHOD( CEntBitSet, operator| ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet opAnd(const int flags)",
		asMETHOD( CEntBitSet, operator& ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "CEntBitSet opXor(const int flags)",
		asMETHOD( CEntBitSet, operator^ ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "int opCom() const",
		asMETHOD( CEntBitSet, operator~ ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool opEquals(const CEntBitSet& in other) const",
		asMETHODPR( CEntBitSet, operator==, ( const CEntBitSet& ) const, bool ), asCALL_THISCALL );

	engine.RegisterObjectMethod(
		pszObjectName, "bool opEquals(const int other) const",
		asMETHODPR( CEntBitSet, operator==, ( const int ) const, bool ), asCALL_THISCALL );
}
