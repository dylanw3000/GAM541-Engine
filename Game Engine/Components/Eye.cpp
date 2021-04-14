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

#include "Eye.h"
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

Eye::Eye() : Component(TYPE_EYE) {
	mAction = 1;
	mTimerLimit = 1000;
	mTimer = mTimerLimit;


}

Eye::~Eye() {

}

void Eye::Update() {


	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));

	if (pC->mIsStunned)
	{
		pT->mVelHoriz = 0;
		pC->mHP -= 1.0;
		pC->mIsStunned = false;
	}


}

void Eye::UpdateTimersAndAngles()
{
	if (mAction == 1)
		mTimer = mTimerLimit = mWaitDuration;
	else if (mAction == 0)
		mTimer = mTimerLimit = mAttackDuration;

	mAttackAngle = mAttackAngleMod;

	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
	if (pS->mIsAnimated)
	{
		if (mAction == 1) {
			if (!pS->mpSpriteAnimator->mIsIdling)
				pS->mpSpriteAnimator->StartIdling();
		}

	}

}

void Eye::Serialize(std::ifstream& InputStream) {

}


void Eye::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {

	if (input[0].HasMember("startAction")) {
		mAction = input[0]["startAction"].GetInt();
	}

	if (input[0].HasMember("waitDuration")) {
		mWaitDuration = input[0]["waitDuration"].GetInt();
	}

	if (input[0].HasMember("attackDuration")) {
		mAttackDuration = input[0]["attackDuration"].GetInt();
	}

	if (input[0].HasMember("attackWidth")) {
		mAttackWidth = input[0]["attackWidth"].GetFloat();
	}

	if (input[0].HasMember("attackAngleMod")) {
		mAttackAngleMod = input[0]["attackAngleMod"].GetFloat();
	}

	if (input[0].HasMember("attackLength")) {
		mAttackLength = input[0]["attackLength"].GetFloat();
	}

	if (input[0].HasMember("damage")) {
		mDamage = input[0]["damage"].GetFloat();
	}

	UpdateTimersAndAngles();

}

void Eye::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::PLAYER_HIT) {

	}
}
