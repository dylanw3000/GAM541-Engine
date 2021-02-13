#pragma once

#include "Component.h"
#include "../AudioManager.h"

class AudioClip : public Component {
public:
	AudioClip();
	~AudioClip();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);
	void Update();

	void PlayOneShot();

public:
	
	float mVolume = 1.0f;

	FMOD::Studio::EventDescription* mEventDescription = NULL;
	FMOD::Studio::EventInstance* mEventInstance = NULL;

private:
private:
};
