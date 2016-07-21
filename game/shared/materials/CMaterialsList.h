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

	/**
	*	Loads materials data from a file.
	*	@param pszFileName Name of the file to load.
	*	@return true on success, false otherwise.
	*/
	bool LoadFromFile( const char* const pszFileName );

	/**
	*	Given texture name, find texture type.
	*	If not found, return type 'concrete'.
	*
	*	NOTE: this routine should ONLY be called if the current texture under the player changes!
	*	@param pszName Texture name.
	*	@param Texture type.
	*/
	char FindTextureType( const char* const pszName ) const;

private:
	void SwapTextures( int i, int j );

	void SortTextures();

	/**
	*	Finds a texture. Searches linearly.
	*	Should only be used during load time.
	*	@param pszName Texture name.
	*	@return Index, or -1 if it wasn't found.
	*/
	int FindTextureLinear( const char* const pszName ) const;

private:
	int m_iTextures = 0;
	char m_szTextureName[ CTEXTURESMAX ][ CBTEXTURENAMEMAX ];
	char m_chTextureType[ CTEXTURESMAX ];

private:
	CMaterialsList( const CMaterialsList& ) = delete;
	CMaterialsList& operator=( const CMaterialsList& ) = delete;
};

#endif //GAME_SHARED_MATERIALS_CMATERIALSLIST_H