#pragma once

#include "Component.h"
#include "../AudioManager.h"
#include <vector>
#include "AudioEvent.h"

class Invincibility : public Component {
public:
	Invincibility();
	~Invincibility();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);
	void Update();

	void SetInvincible(float a);

public:
	bool isInvincible = false;
	

private:
	float invinceTimer = 0.0;
private:
};
