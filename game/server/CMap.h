#ifndef GAME_SERVER_CMAP_H
#define GAME_SERVER_CMAP_H

#include "Color.h"
#include "HudColors.h"

#include "SaveRestore.h"

/**
*	Stores global per-map data.
*/
class CMap final
{
public:
	DECLARE_CLASS_NOBASE( CMap );
	DECLARE_DATADESC_FINAL();

	static CMap* CreateInstance();

	static CMap* GetInstance();

	static void DestroyInstance();

	static bool Exists();

	static CMap* CreateIfNeeded();

	CMap();
	~CMap();

	bool Save( CSave& save );
	bool Restore( CRestore& restore );

	/**
	*	Runs per-frame think operations.
	*/
	void Think();

	/**
	*	Sends Hud colors for the given player if needed.
	*	@param pPlayer Player to send colors to.
	*	@param bForce If true, always send, regardless of conditions.
	*/
	void SendHudColors( CBasePlayer* pPlayer, const bool bForce = false );

private:
	static void SendHudColors( CBasePlayer* pPlayer, const CHudColors& colors );

public:
	/**
	*	@return The primary HUD color.
	*/
	const Color& GetPrimaryHudColor() const { return m_HudColors.m_PrimaryColor; }

	/**
	*	Sets the primary HUD color.
	*	@param color Color to set.
	*/
	void SetPrimaryHudColor( const Color& color )
	{
		m_HudColors.m_PrimaryColor = color;

		m_bUseCustomHudColors = true;
	}

	/**
	*	@return The empty / nearly empty HUD color.
	*/
	const Color& GetEmptyItemHudColor() const { return m_HudColors.m_EmptyItemColor; }

	/**
	*	Sets the empty / nearly empty HUD color.
	*	@param color Color to set.
	*/
	void SetEmptyItemHudColor( const Color& color )
	{
		m_HudColors.m_EmptyItemColor = color;

		m_bUseCustomHudColors = true;
	}

	/**
	*	@return The ammo bar HUD color.
	*/
	const Color& GetAmmoBarHudColor() const { return m_HudColors.m_AmmoBarColor; }

	/**
	*	Sets the ammo bar HUD color.
	*	@param color Color to set.
	*/
	void SetAmmoBarHudColor( const Color& color )
	{
		m_HudColors.m_AmmoBarColor = color;

		m_bUseCustomHudColors = true;
	}

	/**
	*	Resets Hud colors to their defaults.
	*	@param bUpdateClients Whether to update all connected clients.
	*/
	void ResetHudColors();

private:
	/**
	*	Runs right after the constructor. Makes it easier to separate init and setup code.
	*/
	void Create();

private:
	static CMap* m_pInstance;

	//Last time we ran think.
	float m_flPrevFrameTime = 0;

	bool m_bUseCustomHudColors = false;

	float m_flLastHudColorChangeTime = 0;

	CHudColors m_HudColors;

private:
	CMap( const CMap& ) = delete;
	CMap& operator=( const CMap& ) = delete;
};

#endif //GAME_SERVER_CMAP_H