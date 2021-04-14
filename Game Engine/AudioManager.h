#pragma once

#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "common.h"
#include "Components/AudioEvent.h"
#include <vector>



class AudioManager
{
public :

	AudioManager();
	~AudioManager();

	void Update();
	void SetMasterBusVolume(float vol);
	void InitEvents();
	void PlayOneShot(std::string eventName);

public : 
	FMOD_RESULT result;
	FMOD::Studio::System* system = NULL;
	FMOD::System* coreSystem = NULL;
	std::vector<AudioEvent*> mEventList;
};