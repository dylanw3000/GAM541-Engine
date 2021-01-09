/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Controller.h
Purpose: Handles player movement
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "../EventManager.h"

class GameObject;
// #include "..\GameObject.h"

class PlayerHitEvent : public Event {
public:
	PlayerHitEvent();
	~PlayerHitEvent();
public:


private:
private:
};

class Controller : public Component {
public:
	Controller();
	~Controller();

	void Update();

	void Serialize(std::ifstream& InputStream) { }
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>) { }

	void HandleEvent(Event* pEvent);

public:



private:
private:
	// float mVelHoriz, mVelVert;
	int mAcceleration, mMaxSpeed;
	float mSlowMod, mExcessSlowMod;
	int mDashTimer, mDashCooldown;

	int mSwingTime, mSwingTimer, mSwingDelay;
	float mSwingAng, mSwingWidth;
	bool mSwinging, mCleaver;
};
