#pragma once

#include "Component.h"

class Runner : public Component {
public:
	Runner();
	~Runner();

	void Update();

	void Serialize(std::ifstream& InputStream) {} // remnant of old serialization process, prior to using json; TODO strip all associated Serializations out
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

	void HandleEvent(Event* pEvent) {}

public:
	float mTimer;
	float mGravity, mAcc;


private:

};
