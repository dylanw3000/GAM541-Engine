/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.cpp
Purpose: Contains all transformation/translation information for an object
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "Transform.h"
#include "../FrameRateController.h"

extern FrameRateController* gpFRC;

Transform::Transform() : Component(TYPE_TRANSFORM) {
	mPositionX = mPositionY = 0.0f;
	mVelHoriz = mVelVert = 0.0f;
	mSpriteOffsetX = mSpriteOffsetY = 0.0f;
	mAngle = 0.0f;
	mWidth = mHeight = 64.0f;
}

Transform::~Transform() {
	//
}

void Transform::Sprite() {
	//
}

void Transform::Update() {
	mPositionX += mVelHoriz * gpFRC->GetDeltaTime();
	mPositionY += mVelVert * gpFRC->GetDeltaTime();
}

void Transform::Serialize(std::ifstream& InputStream) {
	InputStream >> mPositionX;
	InputStream >> mPositionY;
}

void Transform::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	if (input[0].HasMember("x")) {
		mPositionX = input[0]["x"].GetFloat();
	}

	if (input[0].HasMember("y")) {
		mPositionY = input[0]["y"].GetFloat();
	}

	if (input[0].HasMember("width")) {
		mWidth = input[0]["width"].GetFloat();
	}

	if (input[0].HasMember("height")) {
		mHeight = input[0]["height"].GetFloat();
	}
	
	if (input[0].HasMember("angle")) {
		mAngle = input[0]["angle"].GetFloat();
	}

	if (input[0].HasMember("xoffset")) {
		mSpriteOffsetX = input[0]["xoffset"].GetFloat();
	}

	if (input[0].HasMember("yoffset")) {
		mSpriteOffsetY = input[0]["yoffset"].GetFloat();
	}

	// mPositionX = mPositionY = 5.0f;
}
