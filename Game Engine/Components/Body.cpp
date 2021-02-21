/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Character.cpp
Purpose: For any unit that can be interacted with in combat, handles collision area and health
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_3
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: November 8, 2020
- End Header --------------------------------------------------------*/

#include "Body.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameObjectManager.h"
#include "../ObjectFactory.h"

extern GameObjectManager* gpGameObjectManager;
extern ObjectFactory* gpObjectFactory;

extern bool DEBUG;

Body::Body() : Component(TYPE_BODY) {
	mWidth = mHeight = 20.0f;
	mWall = false;
}

Body::~Body() {
	//
}

void Body::Update() {
	//
}

void Body::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	if (input[0].HasMember("height")) {
		mHeight = input[0]["height"].GetFloat();
	}

	if (input[0].HasMember("width")) {
		mWidth = input[0]["width"].GetFloat();
	}

	if (input[0].HasMember("wall")) {
		mWall = input[0]["wall"].GetBool();
	}

	if (input[0].HasMember("bounce")) {
		mBounce = input[0]["bounce"].GetBool();
	}
}

