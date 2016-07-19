#ifndef GAME_SHARED_MATERIALS_CMATERIALSLIST_H
#define GAME_SHARED_MATERIALS_CMATERIALSLIST_H

#include "MaterialsConst.h"

/**
*	List of materials.
*/
class CMaterialsList final
{
public:
	CMaterialsList() = default;

	bool LoadFromFile( const char* const pszFileName );

	char FindTextureType( const char* const pszName ) const;

private:
	void SwapTextures( int i, int j );

	void SortTextures();

private:
	int m_iTextures = 0;
	char m_szTextureName[ CTEXTURESMAX ][ CBTEXTURENAMEMAX ];
	char m_chTextureType[ CTEXTURESMAX ];

private:
	CMaterialsList( const CMaterialsList& ) = delete;
	CMaterialsList& operator=( const CMaterialsList& ) = delete;
};

#endif //GAME_SHARED_MATERIALS_CMATERIALSLIST_H