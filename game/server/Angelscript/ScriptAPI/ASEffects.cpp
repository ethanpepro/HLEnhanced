#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"

#include "ASEffects.h"

namespace Effects
{
void LightStyle( int iStyle, const std::string& szValue )
{
	if( iStyle < 0 || iStyle >= MAX_LIGHTSTYLES )
	{
		Alert( at_warning, "Effects::LightStyle: Style index \"%d\" out of range [ 0, %d ]!\n", iStyle, MAX_LIGHTSTYLES );
		return;
	}

	g_engfuncs.pfnLightStyle( iStyle, STRING( ALLOC_STRING( szValue.c_str() ) ) );
}
}

void RegisterScriptEffects( asIScriptEngine& engine )
{
	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "Effects" );

	engine.RegisterGlobalFunction(
		"void ParticleEffect(const Vector& in vecOrigin, const Vector& in vecDirection, const uint ulColor, const uint ulCount)",
		asFUNCTION( UTIL_ParticleEffect ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void LightStyle(int iStyle, const string& in szValue)",
		asFUNCTION( Effects::LightStyle ), asCALL_CDECL );

	engine.RegisterGlobalFunction(
		"void StaticDecal(const Vector& in vecOrigin, int decalIndex, int entityIndex, int modelIndex)",
		asFUNCTION( g_engfuncs.pfnStaticDecal ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}
