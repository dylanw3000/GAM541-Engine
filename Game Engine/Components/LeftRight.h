/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: UpDown.h
Purpose: Extremely simple controller to demonstrate NPC behaviour
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"

class GameObject;

enum ACTION_TYPE {
	WALK_RIGHT,
	WAIT_RIGHT,
	WALK_LEFT,
	WAIT_LEFT
};

class LeftRight : public Component {
public:
	LeftRight();
	~LeftRight();

	void Update();

	void Serialize(rapidjson::GenericArray<false, rapidjson::Value> input);
	void Serialize(std::ifstream& InputStream);

	void HandleEvent(Event* pEvent);

public:
	
	int mTimer, mTimerLimit;
	int numActions;
	int mMoveDuration, mWaitDuration;
	int mSpeed;
	unsigned int mAction;
	bool isStunned;
	float visionWidth;
	float visionAngle;
	float visionAngleMod;
	float visionLength;
	float mDamage;


private:
	void UpdateTimersAndAngles();
private:
};
