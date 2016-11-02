#include <string>

#include <angelscript.h>

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "CBasePlayer.h"

#include "ASCBasePlayer.h"

static void RegisterScriptPhysFlag( asIScriptEngine& engine )
{
	const char* const pszObjectName = "PhysFlag";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "ONLADDER", PFLAG_ONLADDER );
	engine.RegisterEnumValue( pszObjectName, "ONSWING", PFLAG_ONSWING );
	engine.RegisterEnumValue( pszObjectName, "ONTRAIN", PFLAG_ONTRAIN );
	engine.RegisterEnumValue( pszObjectName, "ONBARNACLE", PFLAG_ONBARNACLE );
	engine.RegisterEnumValue( pszObjectName, "DUCKING", PFLAG_DUCKING );
	engine.RegisterEnumValue( pszObjectName, "USING", PFLAG_USING );
	engine.RegisterEnumValue( pszObjectName, "OBSERVER", PFLAG_OBSERVER );
	engine.RegisterEnumValue( pszObjectName, "ONROPE", PFLAG_ONROPE );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptTrainFlag( asIScriptEngine& engine )
{
	const char* const pszObjectName = "TrainFlag";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "ACTIVE", TRAIN_ACTIVE );
	engine.RegisterEnumValue( pszObjectName, "NEW", TRAIN_NEW );
	engine.RegisterEnumValue( pszObjectName, "OFF", TRAIN_OFF );
	engine.RegisterEnumValue( pszObjectName, "NEUTRAL", TRAIN_NEUTRAL );
	engine.RegisterEnumValue( pszObjectName, "SLOW", TRAIN_SLOW );
	engine.RegisterEnumValue( pszObjectName, "MEDIUM", TRAIN_MEDIUM );
	engine.RegisterEnumValue( pszObjectName, "FAST", TRAIN_FAST );
	engine.RegisterEnumValue( pszObjectName, "BACK", TRAIN_BACK );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

static void RegisterScriptInputFlag( asIScriptEngine& engine )
{
	const char* const pszObjectName = "InputFlag";

	const std::string szOldNS = engine.GetDefaultNamespace();

	engine.SetDefaultNamespace( pszObjectName );

	engine.RegisterEnum( pszObjectName );

	engine.RegisterEnumValue( pszObjectName, "ATTACK", IN_ATTACK );
	engine.RegisterEnumValue( pszObjectName, "JUMP", IN_JUMP );
	engine.RegisterEnumValue( pszObjectName, "DUCK", IN_DUCK );
	engine.RegisterEnumValue( pszObjectName, "FORWARD", IN_FORWARD );
	engine.RegisterEnumValue( pszObjectName, "BACK", IN_BACK );
	engine.RegisterEnumValue( pszObjectName, "USE", IN_USE );
	engine.RegisterEnumValue( pszObjectName, "CANCEL", IN_CANCEL );
	engine.RegisterEnumValue( pszObjectName, "LEFT", IN_LEFT );
	engine.RegisterEnumValue( pszObjectName, "RIGHT", IN_RIGHT );
	engine.RegisterEnumValue( pszObjectName, "MOVELEFT", IN_MOVELEFT );
	engine.RegisterEnumValue( pszObjectName, "MOVERIGHT", IN_MOVERIGHT );
	engine.RegisterEnumValue( pszObjectName, "ATTACK2", IN_ATTACK2 );
	engine.RegisterEnumValue( pszObjectName, "RUN", IN_RUN );
	engine.RegisterEnumValue( pszObjectName, "RELOAD", IN_RELOAD );
	engine.RegisterEnumValue( pszObjectName, "ALT1", IN_ALT1 );
	engine.RegisterEnumValue( pszObjectName, "SCORE", IN_SCORE );

	engine.SetDefaultNamespace( szOldNS.c_str() );
}

void RegisterScriptPlayerDependencies( asIScriptEngine& engine )
{
	RegisterScriptPhysFlag( engine );
	RegisterScriptTrainFlag( engine );
	RegisterScriptInputFlag( engine );
}
