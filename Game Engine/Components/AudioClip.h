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

class AudioClip : public Component {
public:
	AudioClip();
	~AudioClip();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);
	void Update();

	void PlayOneShot(std::string eventName);

public:
	
	float mVolume = 1.0f;
	int mEventCount;

	std::vector<AudioEvent*> mEventList;

private:
private:
};
