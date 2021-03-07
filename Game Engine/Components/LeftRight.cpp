/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: UpDown.cpp
Purpose: Extremely simple controller to demonstrate NPC behaviour
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "LeftRight.h"
#include "Transform.h"
#include "Sprite.h"
#include "Character.h"
#include "..\GameObject.h"
#include "..\FrameRateController.h"
#include "../EventManager.h"
#include "../CollisionManager.h"
#include "../GameObjectManager.h"

extern GameObjectManager* gpGameObjectManager;
extern CollisionManager* gpCollisionManager;
extern FrameRateController* gpFRC;
extern EventManager* gpEventManager;

LeftRight::LeftRight() : Component(TYPE_LEFT_RIGHT) {
	mAction = WALK_RIGHT;
	numActions = 4;
	mTimerLimit = 1000;
	mTimer = mTimerLimit;
	
}

LeftRight::~LeftRight() {

}

void LeftRight::Update() {
	mTimer -= gpFRC->GetFrameTime();

	if (mTimer <= 0) {
		mAction++;
		mAction %= 4;
		UpdateTimersAndAngles();
		
	}

	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	if (!isStunned && mAction == WALK_LEFT)
		pT->mVelHoriz = -1 * mSpeed;
	else if (!isStunned && mAction == WALK_RIGHT)
		pT->mVelHoriz = 1 * mSpeed;
	else
		pT->mVelHoriz = 0;


	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	//pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY, 0, 10, 0, 150, 0.f, .2, .8, .2, .5, .2, .8, .2, .5);
																										 
	pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY, visionAngle, visionWidth, 20, visionLength, 0, .8, .2, .2, .5, .8, .3, .3, .5);


	
	for (auto pObject : gpGameObjectManager->mGameObjects) {
		Character* pChar = static_cast<Character*>(pObject->GetComponent(TYPE_CHARACTER));
		if (pChar == nullptr || pChar->mFriendly == false)
			continue;

		if (pChar->CollideCirc(pT->mPositionX, pT->mPositionY, visionAngle, visionWidth, 0, visionLength)) {
			pChar->mHP -= .1f;
		}
		

			

	}
	
	


}

void LeftRight::UpdateTimersAndAngles()
{
	if (mAction == WALK_LEFT || mAction == WALK_RIGHT)
		mTimer = mTimerLimit = mMoveDuration;
	else if (mAction == WAIT_RIGHT || mAction == WAIT_LEFT)
		mTimer = mTimerLimit = mWaitDuration;
	if (mAction == WALK_LEFT || mAction == WAIT_LEFT)
		visionAngle = M_PI + visionAngleMod;
	else
		visionAngle = visionAngleMod;

	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
	if (pS->mIsAnimated)
	{
		if (mAction == WALK_LEFT || mAction == WALK_RIGHT) {
			if (!pS->mpSpriteAnimator->mIsRunning)
				pS->mpSpriteAnimator->StartRunning();
		}
		else
		{
			//if (!pS->mpSpriteAnimator->mIsIdling)
				//pS->mpSpriteAnimator->StartIdling();
		}

	}
		

}

void LeftRight::Serialize(std::ifstream& InputStream) {

}


void LeftRight::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {

	if (input[0].HasMember("startAction")) {
		mAction = input[0]["startAction"].GetInt();
	}

	if (input[0].HasMember("moveDuration")) {
		mMoveDuration = input[0]["moveDuration"].GetInt();
	}

	if (input[0].HasMember("waitDuration")) {
		mWaitDuration = input[0]["waitDuration"].GetInt();
	}

	if (input[0].HasMember("speed")) {
		mSpeed = input[0]["speed"].GetInt();
	}

	if (input[0].HasMember("visionWidth")) {
		visionWidth = input[0]["visionWidth"].GetFloat();
	}

	if (input[0].HasMember("visionAngle")) {
		visionAngle = input[0]["visionAngle"].GetFloat();
	}

	if (input[0].HasMember("visionLength")) {
		visionLength = input[0]["visionLength"].GetFloat();
	}

	UpdateTimersAndAngles();

}

void LeftRight::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::PLAYER_HIT) {
		//Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
		//pT->mWidth += 10;
		//pT->mHeight += 10;
	}
}
