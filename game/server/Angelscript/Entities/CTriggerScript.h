#ifndef GAME_SERVER_ANGELSCRIPT_ENTITIES_CTRIGGERSCRIPT_H
#define GAME_SERVER_ANGELSCRIPT_ENTITIES_CTRIGGERSCRIPT_H

#include <vector>

#include <angelscript.h>

#include <Angelscript/util/CASRefPtr.h>

class CTriggerScript : public CBaseDelay
{
public:
	enum class Mode : int
	{
		FIRST		= 0,
		TRIGGER		= FIRST,
		THINK		= 1,

		MODE_COUNT,
		LAST		= MODE_COUNT - 1
	};

	enum class ThinkState : int
	{
		THINKING = 0,
		STARTING = 1,
		STOPPING = 2
	};

	static const char* ThinkStateToString( const ThinkState state );

	/**
	*	Parses the given string and returns a ThinkState constant that represents it.
	*	If pszString is null or doesn't match any constant, returns THINKING.
	*/
	static ThinkState ThinkStateFromString( const char* const pszString );

	/**
	*	If mode is THINK, the entity starts on.
	*/
	static const int SF_START_ON			= 1 << 0;

	/**
	*	Check plugin modules when looking for functions.
	*/
	static const int SF_CHECK_PLUGINS		= 1 << 1;

	/**
	*	Ignore map scripts when looking for functions.
	*/
	static const int SF_IGNORE_MAP_SCRIPTS	= 1 << 2;

	/**
	*	If mode is THINK, pass parameter ThinkState::ThinkState indicating whether the entity is thinking, starting or stopping.
	*	This also results in think functions being called when starting and stopping.
	*/
	static const int SF_THINK_STATES		= 1 << 3;

private:
	using FunctionList_t = std::vector<CASRefPtr<asIScriptFunction>>;

public:
	DECLARE_CLASS( CTriggerScript, CBaseDelay );
	DECLARE_DATADESC();

	bool Restore( CRestore& restore ) override;

	void KeyValue( KeyValueData* pkvd ) override;

	void Spawn() override;

	inline bool IsThinkTrigger() const { return m_Mode == Mode::THINK; }

	inline bool ShouldStartOn() const { return GetSpawnFlags().Any( SF_START_ON ); }

	inline bool ShouldCheckPlugins() const { return GetSpawnFlags().Any( SF_CHECK_PLUGINS ); }

	inline bool ShouldIgnoreMapScripts() const { return GetSpawnFlags().Any( SF_IGNORE_MAP_SCRIPTS ); }

	inline bool ShouldUpdateThinkStates() const { return GetSpawnFlags().Any( SF_THINK_STATES ); }

	/**
	*	@return Next time this entity should think.
	*/
	inline float NextThinkTime() const { return gpGlobals->time + m_flThinkInterval; }

	/**
	*	@return Whether this trigger has a function name to look up.
	*/
	inline bool HasFunctionName() const { return !FStringNull( m_iszFunctionName ); }

	/**
	*	Use method for trigger mode. Calls the functions on trigger.
	*/
	void TriggerUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue );

	/**
	*	Use method for think mode. Toggles thinking on or off.
	*/
	void ToggleUse( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue );

	/**
	*	Think method for calling script functions every time an interval passes.
	*/
	void CallScriptThink();

private:
	/**
	*	Find functions in all modules and store references to them.
	*	Only plugins with lifetime MAP or greater are considered for calls.
	*/
	void FindFunctions();

	/**
	*	Calls think functions, passes the given state to it.
	*	Can be called regardless of whether SF_THINK_STATES is set.
	*/
	void CallThink( const ThinkState state );

private:
	string_t m_iszFunctionName = iStringNull;
	float m_flThinkInterval = 0;

	Mode m_Mode = Mode::TRIGGER;

	//Not restored: looked up again.
	FunctionList_t m_FunctionList;
};

#endif //GAME_SERVER_ANGELSCRIPT_ENTITIES_CTRIGGERSCRIPT_H
