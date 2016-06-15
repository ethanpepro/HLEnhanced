#ifndef GAME_SERVER_CMAP_H
#define GAME_SERVER_CMAP_H

/**
*	Stores global per-map data.
*/
class CMap final
{
public:
	static CMap* CreateInstance();

	static CMap* GetInstance();

	static void DestroyInstance();

	static bool Exists();

	static CMap* CreateIfNeeded();

	CMap();
	~CMap();

private:
	/**
	*	Runs right after the constructor. Makes it easier to separate init and setup code.
	*/
	void Create();

private:
	static CMap* m_pInstance;

private:
	CMap( const CMap& ) = delete;
	CMap& operator=( const CMap& ) = delete;
};

#endif //GAME_SERVER_CMAP_H