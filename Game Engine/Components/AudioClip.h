#pragma once

#include "Component.h"

class AudioClip : public Component {
public:
	AudioClip();
	~AudioClip();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);
	void Update();

public:
	std::string bankName;
	std::string eventName;

private:
private:
};
