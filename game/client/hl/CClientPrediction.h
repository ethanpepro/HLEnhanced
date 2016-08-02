#ifndef GAME_CLIENT_HL_CCLIENTPREDICTION_H
#define GAME_CLIENT_HL_CCLIENTPREDICTION_H

class CBasePlayerWeapon;

/**
*	Handles the client's weapon prediction.
*/
class CClientPrediction final
{
public:
	CClientPrediction() = default;
	~CClientPrediction() = default;

	/**
	*	@return The local player.
	*/
	CBasePlayer* GetLocalPlayer() { return m_pPlayer; }

	/**
	*	Called when a map has started loading.
	*/
	void MapInit();

	/**
	*	Creates a new entity.
	*/
	entvars_t* CreateEntity();

	/**
	*	Adds a weapon.
	*/
	void AddWeapon( CBasePlayerWeapon* pWeapon );

	/**
	*	Sets up weapons.
	*/
	void SetupWeapons();

	void WeaponsPostThink( local_state_s *from, local_state_s *to, usercmd_t *cmd, double time, unsigned int random_seed );

private:
	// Pool of client side entities/entvars_t
	//Weapons + local player - Solokiller
	entvars_t m_Entvars[ MAX_WEAPONS + 1 ] = {};
	size_t m_uiNumAllocatedEnts = 0;

	CBasePlayerWeapon* m_pWeapons[ MAX_WEAPONS ] = {};

	CBasePlayer* m_pPlayer = nullptr;

private:
	CClientPrediction( const CClientPrediction& ) = delete;
	CClientPrediction& operator=( const CClientPrediction& ) = delete;
};

extern CClientPrediction g_Prediction;

#endif //GAME_CLIENT_HL_CCLIENTPREDICTION_H