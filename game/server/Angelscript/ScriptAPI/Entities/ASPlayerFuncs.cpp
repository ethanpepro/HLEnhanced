#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CBasePlayer.h"

#include "ASPlayerFuncs.h"

void RegisterScriptPlayerFuncs( asIScriptEngine& engine )
{
	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( "Player" );

	engine.RegisterGlobalFunction(
		"int TrainSpeed(int iSpeed, int iMax)",
		asFUNCTION( TrainSpeed ), asCALL_CDECL );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}
