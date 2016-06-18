/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#ifndef GAME_SERVER_ENTITIES_TRIGGERS_CBASETRIGGER_H
#define GAME_SERVER_ENTITIES_TRIGGERS_CBASETRIGGER_H

class CBaseTrigger : public CBaseToggle
{
public:
	DECLARE_CLASS( CBaseTrigger, CBaseToggle );

	void EXPORT TeleportTouch( CBaseEntity *pOther );
	void KeyValue( KeyValueData *pkvd ) override;
	void EXPORT MultiTouch( CBaseEntity *pOther );
	void EXPORT CDAudioTouch( CBaseEntity *pOther );
	void ActivateMultiTrigger( CBaseEntity *pActivator );
	void EXPORT MultiWaitOver();
	void EXPORT CounterUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void EXPORT ToggleUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void InitTrigger();

	virtual int	ObjectCaps() const override { return CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
};

#endif //GAME_SERVER_ENTITIES_TRIGGERS_CBASETRIGGER_H