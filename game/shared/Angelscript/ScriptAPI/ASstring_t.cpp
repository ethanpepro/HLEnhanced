#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"

#include "ASstring_t.h"

void string_t_DefaultConstruct( string_t* pString )
{
	*pString = iStringNull;
}

void string_t_CopyConstruct( string_t* pString, const string_t& str )
{
	*pString = str;
}

void string_t_ASStringConstruct( string_t* pString, const std::string& str )
{
	*pString = ALLOC_STRING( str.c_str() );
}

string_t& string_t_Assign_string_t( string_t* pThis, const string_t& str )
{
	*pThis = str;

	return *pThis;
}

string_t& string_t_Assign_CString( string_t* pThis, const std::string& str )
{
	*pThis = ALLOC_STRING( str.c_str() );

	return *pThis;
}

std::string string_t_ToString( const string_t& str )
{
	return STRING( str );
}

void RegisterScriptstring_t( asIScriptEngine& engine )
{
	const char* pszObjectName = "string_t";

	engine.RegisterObjectType(
		pszObjectName, 
		sizeof( string_t ), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_PRIMITIVE );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void string_t()",
		asFUNCTION( string_t_DefaultConstruct ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void string_t(const string_t& in str)",
		asFUNCTION( string_t_CopyConstruct ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectBehaviour(
		pszObjectName, asBEHAVE_CONSTRUCT, "void string_t(const string& in str)",
		asFUNCTION( string_t_ASStringConstruct ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "string_t& opAssign(const string_t& in str)",
		asFUNCTION( string_t_Assign_string_t ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "string_t& opAssign(const string& in str)",
		asFUNCTION( string_t_Assign_CString ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "string opImplConv() const",
		asFUNCTION( string_t_ToString ), asCALL_CDECL_OBJFIRST );

	engine.RegisterObjectMethod(
		pszObjectName, "string ToString() const",
		asFUNCTION( string_t_ToString ), asCALL_CDECL_OBJFIRST );
}
