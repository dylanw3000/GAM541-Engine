/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Transform.h
Purpose: Contains all transformation/translation information for an object
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "../Matrix2D.h"

class GameObject;

class Transform : public Component {
public:
	Transform();
	~Transform();

	void Sprite();
	void Update();

	void Serialize(std::ifstream& InputStream);
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

public:
	float mPositionX, mPositionY;
	float mVelHoriz, mVelVert;
	float mSpriteOffsetX, mSpriteOffsetY;
	float mAngle;

	float mWidth, mHeight;
	// Matrix2D mTransformMatrix;


private:
	// float mPositionX, mPositionY;
};
