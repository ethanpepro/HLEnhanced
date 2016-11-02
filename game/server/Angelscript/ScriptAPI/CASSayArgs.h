#ifndef GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CASSAYARGS_H
#define GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CASSAYARGS_H

#include <string>

#include <Angelscript/util/CASBaseClass.h>

class CBasePlayer;
class CASCommand;

class CASSayArgs : public CASRefCountedBaseClass
{
public:
	CASSayArgs( CBasePlayer* pPlayer, const SayType::SayType sayType, const char* pszString );
	~CASSayArgs();

	void Release() const;

	CBasePlayer* GetPlayer() const { return m_pPlayer; }

	SayType::SayType GetSayType() const { return m_SayType; }

	const std::string& GetCommand() const { return m_szCommand; }

	const CASCommand* GetArguments() const;

	bool ShouldHide() const { return m_bShouldHide; }

	void SetShouldHide( bool bShouldHide ) { m_bShouldHide = bShouldHide; }

private:
	CBasePlayer* m_pPlayer;					//The player that is saying something
	const SayType::SayType m_SayType;		//ClientSayType
	const std::string m_szCommand;			//The entire string being sent
	CASCommand* m_pArguments;				//Arguments in the string
	bool m_bShouldHide = false;				//Whether the string should be hidden from chat or not

private:
	CASSayArgs( const CASSayArgs& ) = delete;
	CASSayArgs& operator=( const CASSayArgs& ) = delete;
};

class asIScriptEngine;

/**
*	Registers the CSayArgs class and the SayType enum.
*	@param engine Script engine.
*/
void RegisterScriptCSayArgs( asIScriptEngine& engine );

#endif //GAME_SERVER_ANGELSCRIPT_SCRIPTAPI_CASSAYARGS_H
