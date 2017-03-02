#include "hud.h"
#include "cl_util.h"

#include "CHudList.h"

#include "vgui_TeamFortressViewport.h"

#include "particleman.h"
extern IParticleMan *g_pParticleMan;

#include "effects/CEnvironment.h"

#if USE_VGUI2
#include <vgui_controls/Panel.h>
#endif

#include "CBaseHud.h"

float HUD_GetFOV();
int CL_ButtonBits( int );
void CL_ResetButtonBits( int bits );

extern cvar_t* sensitivity;
cvar_t* cl_lw = nullptr;

namespace
{
static CBaseHud* g_pHud = nullptr;
}

CBaseHud& Hud()
{
	ASSERT( g_pHud );

	return *g_pHud;
}

void SetHud( CBaseHud* pHud, bool bDeleteOldHud )
{
	if( bDeleteOldHud && g_pHud )
		delete g_pHud;

	g_pHud = pHud;
}

CBaseHud::CBaseHud()
{
}

CBaseHud::~CBaseHud()
{
	// cleans up memory allocated for sprite arrays
	delete[] m_pSprites;
}

void CBaseHud::Init()
{
	// In case we get messages before the first update -- time will be valid
	m_flTime = 1.0;

	m_iFOV = 0;

	default_fov = CVAR_CREATE( "default_fov", "90", 0 );
	hud_takesshots = CVAR_CREATE( "hud_takesshots", "0", FCVAR_ARCHIVE );	// controls whether or not to automatically take screenshots at the end of a round
	cl_lw = gEngfuncs.pfnGetCvarPointer( "cl_lw" );
	CVAR_CREATE( "zoom_sensitivity_ratio", "1.2", 0 );
}

void CBaseHud::VidInit()
{
	m_scrinfo.iSize = sizeof( m_scrinfo );
	GetScreenInfo( &m_scrinfo );

	// ----------
	// Load Sprites
	// ---------
	//	m_hsprFont = LoadSprite("sprites/%d_font.spr");

	if( ScreenWidth < 640 )
		m_iResolution = 320;
	else
		m_iResolution = 640;

	// Only load this once
	//TODO: reload it so we can load the Hud from different locations in the future. - Solokiller
	if( !m_pSpriteList )
	{
		// we need to load the hud.txt, and all sprites within
		m_pSpriteList = SPR_GetList( "sprites/hud.txt", &m_iSpriteCountAllRes );

		if( m_pSpriteList )
		{
			// count the number of sprites of the appropriate res
			m_iSpriteCount = 0;
			client_sprite_t *p = m_pSpriteList;
			int j;
			for( j = 0; j < m_iSpriteCountAllRes; j++ )
			{
				if( p->iRes == m_iResolution )
					m_iSpriteCount++;
				p++;
			}

			// allocated memory for sprite arrays
			m_pSprites = new HudSprite_t[ m_iSpriteCount ];

			p = m_pSpriteList;
			int index = 0;
			for( j = 0; j < m_iSpriteCountAllRes; j++ )
			{
				if( p->iRes == m_iResolution )
				{
					char sz[ 256 ];
					V_sprintf_safe( sz, "sprites/%s.spr", p->szSprite );
					m_pSprites[ index ].hSprite = SPR_Load( sz );
					m_pSprites[ index ].rect = p->rc;
					strncpy( m_pSprites[ index ].szName, p->szName, sizeof( m_pSprites[ index ].szName ) );

					index++;
				}

				p++;
			}
		}
	}
	else
	{
		// we have already have loaded the sprite reference from hud.txt, but
		// we need to make sure all the sprites have been loaded (we've gone through a transition, or loaded a save game)
		client_sprite_t *p = m_pSpriteList;
		int index = 0;
		for( int j = 0; j < m_iSpriteCountAllRes; j++ )
		{
			if( p->iRes == m_iResolution )
			{
				char sz[ 256 ];
				V_sprintf_safe( sz, "sprites/%s.spr", p->szSprite );
				m_pSprites[ index ].hSprite = SPR_Load( sz );
				index++;
			}

			p++;
		}
	}

	LoadSprites();

	HudList().ForEachHudElem( &CHudElement::VidInit );
}

void CBaseHud::LoadSprites()
{
	// assumption: number_1, number_2, etc, are all listed and loaded sequentially
	m_HUD_number_0 = GetSpriteIndex( "number_0" );

	ASSERT( m_HUD_number_0 != INVALID_SPRITE_INDEX );

	const auto& rect = GetSpriteRect( m_HUD_number_0 );

	m_iFontHeight = rect.bottom - rect.top;
}

void CBaseHud::InitHud()
{
	// prepare all hud data
	HudList().ForEachHudElem( &CHudElement::InitHUDData );

	g_Environment.Initialize();

	if( g_pParticleMan )
		g_pParticleMan->ResetParticles();
}

void CBaseHud::ResetHud()
{
	// clear all hud data
	HudList().ForEachHudElem( &CHudElement::Reset );

	// reset sensitivity
	SetSensitivity( 0 );
}

bool CBaseHud::Redraw( float flTime, bool intermission )
{
	m_flOldTime = m_flTime;	// save time of previous redraw
	m_flTime = flTime;
	m_flTimeDelta = ( double ) m_flTime - m_flOldTime;

	// Clock was reset, reset delta
	if( m_flTimeDelta < 0 )
		m_flTimeDelta = 0;

	// Bring up the scoreboard during intermission
	if( gViewPort )
	{
		if( m_bIntermission && !intermission )
		{
			// Have to do this here so the scoreboard goes away
			m_bIntermission = intermission;
			gViewPort->HideCommandMenu();
			gViewPort->HideScoreBoard();
			gViewPort->UpdateSpectatorPanel();
		}
		else if( !m_bIntermission && intermission )
		{
			m_bIntermission = intermission;
			gViewPort->HideCommandMenu();
			gViewPort->HideVGUIMenu();
			gViewPort->ShowScoreBoard();
			gViewPort->UpdateSpectatorPanel();

			// Take a screenshot if the client's got the cvar set
			if( CVAR_GET_FLOAT( "hud_takesshots" ) != 0 )
				m_flSnapshotTime = flTime + 1.0;	// Take a screenshot in a second
		}
	}

	if( m_flSnapshotTime && m_flSnapshotTime < flTime )
	{
		gEngfuncs.pfnClientCmd( "snapshot\n" );
		m_flSnapshotTime = 0;
	}

	m_bIntermission = intermission;

	return DoDraw( flTime, intermission );
}

void CBaseHud::DrawHudElements( float flTime, HudElementEvaluatorFn evaluatorFn, void* pUserData )
{
	ASSERT( evaluatorFn );

	auto count = HudList().GetElementCount();

	for( decltype( count ) index = 0; index < count; ++index )
	{
		auto pElem = HudList().GetElementByIndex( index );

#if USE_VGUI2
		// Visible?
		bool visible = pElem->ShouldDraw();

		pElem->SetActive( visible );

		// If it's a vgui panel, hide/show as appropriate
		vgui2::Panel *pPanel = dynamic_cast<vgui2::Panel*>( pElem );
		if( pPanel && pPanel->IsVisible() != visible )
		{
			pPanel->SetVisible( visible );
		}
		else if( !pPanel )
		{
			// All HUD elements should now derive from vgui!!!
			//TODO - Solokiller
			//Assert( false );
		}

		if( visible )
		{
			pElem->ProcessInput();
		}
#endif

		if( evaluatorFn( pElem, pUserData ) )
			pElem->Draw( flTime );
	}
}

bool CBaseHud::UpdateClientData( client_data_t* cdata )
{
	auto bChanged = PreThinkUpdateClient( cdata );

	Think();

	bChanged = PostThinkUpdateClient( cdata ) || bChanged;

	return bChanged;
}

bool CBaseHud::PreThinkUpdateClient( client_data_t* cdata )
{
	m_vecOrigin = cdata->origin;
	m_vecAngles = cdata->viewangles;

	m_iKeyBits = CL_ButtonBits( 0 );
	m_iWeaponBits = cdata->iWeaponBits;

	return false;
}

bool CBaseHud::PostThinkUpdateClient( client_data_t* cdata )
{
	cdata->fov = GetFOV();

	CL_ResetButtonBits( GetKeyBits() );

	return true;
}

void CBaseHud::Think()
{
	m_scrinfo.iSize = sizeof( m_scrinfo );
	GetScreenInfo( &m_scrinfo );

	auto count = HudList().GetElementCount();

	for( decltype( count ) index = 0; index < count; ++index )
	{
		auto pElem = HudList().GetElementByIndex( index );

		if( pElem->GetFlags() & HUD_ACTIVE )
			pElem->Think();
	}

	UpdateFOV( HUD_GetFOV(), true );
}

void CBaseHud::UpdateFOV( int iNewFOV, bool bForce )
{
	if( iNewFOV == 0 )
	{
		SetFOV( default_fov->value );
	}
	else
	{
		SetFOV( iNewFOV );
	}

	// the clients fov is actually set in the client data update section of the hud

	// Set a new sensitivity
	if( m_iFOV == default_fov->value )
	{
		// reset to saved sensitivity
		SetSensitivity( 0 );
	}
	else
	{
		// set a new sensitivity that is proportional to the change from the FOV default
		SetSensitivity( sensitivity->value * ( ( float ) iNewFOV / ( float ) default_fov->value ) * CVAR_GET_FLOAT( "zoom_sensitivity_ratio" ) );
	}

	if( !bForce )
	{
		// think about default fov
		if( m_iFOV == 0 )
		{  // only let players adjust up in fov,  and only if they are not overriden by something else
			SetFOV( max( default_fov->value, 90.0f ) );
		}
	}

	if( gEngfuncs.IsSpectateOnly() )
	{
		if( auto pSpectator = GETHUDCLASS( CHudSpectator ) )
			SetFOV( pSpectator->GetFOV() );	// default_fov->value;
		else
			SetFOV( default_fov->value );
	}
}

int CBaseHud::DrawHudString( int xpos, int ypos, int iMaxX, char *szIt, int r, int g, int b )
{
	return xpos + gEngfuncs.pfnDrawString( xpos, ypos, szIt, r, g, b );
}

int CBaseHud::DrawHudNumberString( int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b )
{
	char szString[ 32 ];
	V_sprintf_safe( szString, "%d", iNumber );
	return DrawHudStringReverse( xpos, ypos, iMinX, szString, r, g, b );
}

// draws a string from right to left (right-aligned)
int CBaseHud::DrawHudStringReverse( int xpos, int ypos, int iMinX, char *szString, int r, int g, int b )
{
	return xpos - gEngfuncs.pfnDrawStringReverse( xpos, ypos, szString, r, g, b );
}

int CBaseHud::DrawHudNumber( int x, int y, int iFlags, int iNumber, int r, int g, int b )
{
	int iWidth = GetSpriteRect( GetHudNumber0Index() ).right - GetSpriteRect( GetHudNumber0Index() ).left;
	int k;

	if( iNumber > 0 )
	{
		// SPR_Draw 100's
		if( iNumber >= 100 )
		{
			k = iNumber / 100;
			SPR_Set( GetSprite( GetHudNumber0Index() + k ), r, g, b );
			SPR_DrawAdditive( 0, x, y, &GetSpriteRect( GetHudNumber0Index() + k ) );
			x += iWidth;
		}
		else if( iFlags & ( DHN_3DIGITS ) )
		{
			//SPR_DrawAdditive( 0, x, y, &rc );
			x += iWidth;
		}

		// SPR_Draw 10's
		if( iNumber >= 10 )
		{
			k = ( iNumber % 100 ) / 10;
			SPR_Set( GetSprite( GetHudNumber0Index() + k ), r, g, b );
			SPR_DrawAdditive( 0, x, y, &GetSpriteRect( GetHudNumber0Index() + k ) );
			x += iWidth;
		}
		else if( iFlags & ( DHN_3DIGITS | DHN_2DIGITS ) )
		{
			//SPR_DrawAdditive( 0, x, y, &rc );
			x += iWidth;
		}

		// SPR_Draw ones
		k = iNumber % 10;
		SPR_Set( GetSprite( GetHudNumber0Index() + k ), r, g, b );
		SPR_DrawAdditive( 0, x, y, &GetSpriteRect( GetHudNumber0Index() + k ) );
		x += iWidth;
	}
	else if( iFlags & DHN_DRAWZERO )
	{
		SPR_Set( GetSprite( GetHudNumber0Index() ), r, g, b );

		// SPR_Draw 100's
		if( iFlags & ( DHN_3DIGITS ) )
		{
			//SPR_DrawAdditive( 0, x, y, &rc );
			x += iWidth;
		}

		if( iFlags & ( DHN_3DIGITS | DHN_2DIGITS ) )
		{
			//SPR_DrawAdditive( 0, x, y, &rc );
			x += iWidth;
		}

		// SPR_Draw ones

		SPR_DrawAdditive( 0, x, y, &GetSpriteRect( GetHudNumber0Index() ) );
		x += iWidth;
	}

	return x;
}


int CBaseHud::GetNumWidth( int iNumber, int iFlags ) const
{
	if( iFlags & ( DHN_3DIGITS ) )
		return 3;

	if( iFlags & ( DHN_2DIGITS ) )
		return 2;

	if( iNumber <= 0 )
	{
		if( iFlags & ( DHN_DRAWZERO ) )
			return 1;
		else
			return 0;
	}

	if( iNumber < 10 )
		return 1;

	if( iNumber < 100 )
		return 2;

	return 3;

}

void CBaseHud::InitHudElements()
{
	HudList().InitHudElements( *this );
}

void CBaseHud::GameShutdown()
{
	HudList().GameShutdown();

	//Free the list so we don't leak memory.
	if( m_pSpriteList )
	{
		gEngfuncs.COM_FreeFile( m_pSpriteList );
		m_pSpriteList = nullptr;
	}
}

HSPRITE CBaseHud::GetSprite( int index ) const
{
	ASSERT( index >= 0 && index < m_iSpriteCount );

	return ( index < 0 ) ? INVALID_HSPRITE : m_pSprites[ index ].hSprite;
}

const wrect_t& CBaseHud::GetSpriteRect( int index ) const
{
	ASSERT( index >= 0 && index < m_iSpriteCount );

	return m_pSprites[ index ].rect;
}

int CBaseHud::GetSpriteIndex( const char* SpriteName ) const
{
	ASSERT( SpriteName );

	// look through the loaded sprite name list for SpriteName
	for( int i = 0; i < m_iSpriteCount; i++ )
	{
		if( strncmp( SpriteName, m_pSprites[ i ].szName, MAX_SPRITE_NAME_LENGTH ) == 0 )
			return i;
	}

	return INVALID_SPRITE_INDEX; // invalid sprite
}