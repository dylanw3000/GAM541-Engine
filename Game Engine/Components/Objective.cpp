/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Slime.cpp
Purpose: Handles movement patterns for a "slime" object
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "Objective.h"
#include "..\GameObjectManager.h"
#include "..\GameObject.h"
#include "../EventManager.h"
#include "../CollisionManager.h"
#include "../EventManager.h"
#include "Component.h"
#include "Transform.h"
#include "Character.h"

extern GameObjectManager* gpGameObjectManager;
extern EventManager* gpEventManager;

extern bool DEBUG;

Objective::Objective() : Component(TYPE_OBJECTIVE) {
	// mVert = 0;
	// mHoriz = 0;
	mRadius = 10.0f;
	mCompleted = false;
}

Objective::~Objective() {
	//
}

void Objective::Update() {
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));

	/*
	pT->mVelHoriz = 0;
	pT->mVelVert = 0;
	pT->mSpriteOffsetY = 0.0f;
	pT->mAngle = 0.0f;
	*/
	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Character* pChar = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
		if (pChar == nullptr || pChar->mFriendly == false)
			continue;

		Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
		if (pow(pT->mPositionX - pTrans->mPositionX, 2) + pow(pT->mPositionY - pTrans->mPositionY, 2) <= pow(pChar->mRadius + mRadius, 2)) {
			mCompleted = true;
		}
	}

	if (mCompleted)
	{
		pT->mVelHoriz = .01f;
	}

	// pT->mPositionX += pT->mVelHoriz * gpFRC->GetDeltaTime();
	// pT->mPositionY += pT->mVelVert * gpFRC->GetDeltaTime();

	if (DEBUG) {
		pC->AddRectColor(pT->mPositionX + 32, pT->mPositionY, 3.14159 / 2, 64, 8, 1, .4, .4, .8, 1, .2, .2, .8, 1);
	}
}

void Objective::Serialize(std::ifstream& InputStream) {
	InputStream >> mRadius;
	mCompleted = false;

}

void Objective::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {

	if (input[0].HasMember("radius")) {
		mRadius = input[0]["radius"].GetInt();
	}

	gpEventManager->Subscribe(EventType::PLAYER_HIT, mpOwner);

	mCompleted = false;
}

void Objective::HandleEvent(Event* pEvent) {
}
