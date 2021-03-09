#pragma once

#include "Component.h"

class Body : public Component {
public:
	Body();
	~Body();

	void Update();

	void Serialize(std::ifstream& InputStream) {} // remnant of old serialization process, prior to using json; TODO strip all associated Serializations out
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

	void HandleEvent(Event* pEvent) {}

public:
	float mWidth, mHeight;
	bool mWall, mBounce, mSemisolid;
	bool mFriendly;



private:

};
