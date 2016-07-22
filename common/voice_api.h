/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
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
#ifndef COMMON_VOICE_API_H
#define COMMON_VOICE_API_H

enum class VoiceTweakControl
{
	MICROPHONE_VOLUME	= 0,

	/**
	*	The scale to multiple other players' voice with.
	*/
	OTHER_SPEAKER_SCALE = 1,
	MIC_BOOST			= 2
};

/**
*	Voice tweak API.
*/
struct IVoiceTweak
{
	/**
	*	Starts transmitting voice.
	*	@return Whether voice was not transmitting and started transmitting.
	*/
	int ( *StartVoiceTweakMode )();

	/**
	*	Stops transmitting voice.
	*/
	void ( *EndVoiceTweakMode )();

	/**
	*	Sets the value for the given control.
	*	@param control Control to set.
	*	@param value Value to set.
	*/
	void ( *SetControlFloat )( VoiceTweakControl control, float value );

	/**
	*	Gets the value for the given control.
	*	@param control Control to get.
	*	@return Value.
	*/
	float ( *GetControlFloat )( VoiceTweakControl control );

	/**
	*	@return The speaking volume.
	*/
	int ( *GetSpeakingVolume )();
};

#endif //COMMON_VOICE_API_H