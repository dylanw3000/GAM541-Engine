#pragma once

#include "fmod.hpp"
#include "fmod_studio.hpp"
#include "common.h"

class AudioManager
{
public :

	AudioManager();
	~AudioManager();

	void Update();

public : 
	FMOD_RESULT result;
	FMOD::Studio::System* system = NULL;
	FMOD::System* coreSystem = NULL;
};