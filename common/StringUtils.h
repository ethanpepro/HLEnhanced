#ifndef COMMON_STRINGUTILS_H
#define COMMON_STRINGUTILS_H

#include <cctype>
#include <cstring>
#include <functional>
#include <string>

#include "Platform.h"

/**
*	String hashing function
*	Ripped from the MSVC std::hash<std::string> implementation
*/
inline size_t StringHash( const char* pszString )
{
	if( !pszString )
		pszString = "";

	size_t _Val = 2166136261U;
	size_t _First = 0;
	size_t _Last = strlen( pszString );
	size_t _Stride = 1 + _Last / 10;

	for( ; _First < _Last; _First += _Stride )
		_Val = 16777619U * _Val ^ ( size_t ) pszString[ _First ];
	return ( _Val );
}

/**
*	String hashing function
*	Ripped from the MSVC std::hash<std::string> implementation
*	Case insensitive
*/
inline size_t StringHashI( const char* pszString )
{
	if( !pszString )
		pszString = "";

	size_t _Val = 2166136261U;
	size_t _First = 0;
	size_t _Last = strlen( pszString );
	size_t _Stride = 1 + _Last / 10;

	for( ; _First < _Last; _First += _Stride )
		_Val = 16777619U * _Val ^ ( size_t ) tolower( pszString[ _First ] );
	return ( _Val );
}

/**
*	Functor for char* hashing.
*/
template<size_t ( *HASHER )( const char* )>
struct BaseRawCharHash : public std::unary_function<const char*, size_t>
{
	size_t operator()( const char* pszString ) const
	{
		return HASHER( pszString );
	}
};

typedef BaseRawCharHash<StringHash> RawCharHash;
typedef BaseRawCharHash<StringHashI> RawCharHashI;

template<int ( *COMPARE )( const char*, const char* )>
struct BaseRawCharEqualTo : public std::binary_function<const char*, const char*, bool>
{
	bool operator()( const char* pszLHS, const char* pszRHS ) const
	{
		return COMPARE( pszLHS, pszRHS ) == 0;
	}
};

typedef BaseRawCharEqualTo<strcmp> RawCharEqualTo;
typedef BaseRawCharEqualTo<stricmp> RawCharEqualToI;

/**
*	Case insensitive hash for std::string
*/
inline size_t StdStringHashI( const std::string& szString )
{
	return StringHashI( szString.c_str() );
}

/**
*	Functor for case insensitive std::string hashing.
*/
struct CStdStringHashI : public std::unary_function<const std::string&, size_t>
{
	size_t operator()( const std::string& szString ) const
	{
		return StdStringHashI( szString );
	}
};

/**
*	Functor for case insensitive std::string comparison.
*/
struct CStdStringEqualToI : public std::binary_function<const std::string&, const std::string&, bool>
{
	bool operator()( const std::string& szLHS, const std::string& szRHS ) const
	{
		return stricmp( szLHS.c_str(), szRHS.c_str() ) == 0;
	}
};

#endif //COMMON_STRINGUTILS_H