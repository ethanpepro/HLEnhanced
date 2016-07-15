#ifndef GAME_SHARED_CBASEGAMEINTERFACE_H
#define GAME_SHARED_CBASEGAMEINTERFACE_H

/**
*	Mixin class for shared game interface code.
*/
class CBaseGameInterface
{
public:
	/**
	*	Constructor.
	*/
	CBaseGameInterface() = default;

	/**
	*	Destructor.
	*/
	~CBaseGameInterface() = default;

protected:
	/**
	*	Initializes common code.
	*	@return true on success, false on failure.
	*/
	bool InitializeCommon();

	/**
	*	Shuts down common code.
	*/
	void ShutdownCommon();

	/**
	*	Initializes the filesystem pointer.
	*	@return true on success, false on failure.
	*/
	bool InitFileSystem();

	/**
	*	Shuts down the filesystem.
	*/
	void ShutdownFileSystem();

private:
	CBaseGameInterface( const CBaseGameInterface& ) = delete;
	CBaseGameInterface& operator=( const CBaseGameInterface& ) = delete;
};

#endif //GAME_SHARED_CBASEGAMEINTERFACE_H