#pragma once

#include "Component.h"
#include "../AudioManager.h"
#include <vector>

class AudioEvent
{
public:
	AudioEvent();
	~AudioEvent();

	void SetData(rapidjson::Value& input);

	std::string mEventName;
	float mEventVolume;
	
	FMOD::Studio::EventDescription* mEventDescription = NULL;
	FMOD::Studio::EventInstance* mEventInstance = NULL;
};