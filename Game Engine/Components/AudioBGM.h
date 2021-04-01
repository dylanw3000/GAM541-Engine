#pragma once

#include "Component.h"
#include "../AudioManager.h"
#include <vector>
#include "AudioEvent.h"

class AudioBGM : public Component {
public:
	AudioBGM();
	~AudioBGM();

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
