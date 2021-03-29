#include "StealthModerator.h"
#include <string>

#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Character.h"
#include "Components/Objective.h"
#include "Components/Body.h"
#include "Components/Transform.h"

#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "ObjectFactory.h"
#include "FrameRateController.h"

extern ResourceManager* gpResourceManager;
extern GameObjectManager* gpGameObjectManager;
extern ObjectFactory* gpObjectFactory;
extern FrameRateController* gpFRC;

extern int gGameType;

StealthModerator::StealthModerator() {
	mStage = 0;
	mTimer = 0.0f;
	mTransition = true;
	mTransitionTimer = mTransitionTimerLimit = 1200;
	mManualOverride = false;
}

StealthModerator::~StealthModerator() {
	//
}

void StealthModerator::Update() {
	if (gGameType == 2 || mStage == 0 || mStage == 99 || mStage == 666) {
		return;
	}

	bool clearLevel = true;
	bool playerAlive = false;
	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
		if (pC == nullptr) continue; 
		if (pC->mFriendly == true) 
		{
			playerAlive = true;
		}

		
	}

	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Objective* pO = static_cast<Objective*>(pGO->GetComponent(TYPE_OBJECTIVE));
		if (pO == nullptr) continue;
		if (pO->mCompleted == false)
		{
			clearLevel = false;
		}
	}

	if (!playerAlive) {
		mStage = 666;
		return;
	}

	if (clearLevel || mManualOverride) {

		mTransitionTimer -= gpFRC->GetFrameTime();

		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
			if (pC == nullptr) continue;
			if (pC->mFriendly == true)
			{
				Transform* pT = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
				Body* pB = static_cast<Body*>(pGO->GetComponent(TYPE_BODY));
				pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY - pB->mHeight / 2.f, 0, M_PI, 1500 * ((float)mTransitionTimer/mTransitionTimerLimit), 1500, 0, 0, 0, 0, .7, 0, 0, 0, .7);
			}
		}

		if (mTransitionTimer <= 0) {

			mStage++;
			if (mStage <= 4) {
				gpGameObjectManager->~GameObjectManager();
				gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
				mTransitionTimer = mTransitionTimerLimit;
				mManualOverride = false;
			}
			else {
				mStage = 99;
			}
		}


		
		
	}
	/*
	if (gameType == 2 || mStage == 0 || mStage == 99 || mStage == 666) {
		return;
	}

	bool nextLevel = false;
	bool prevLevel = false;
	bool playerAlive = false;
	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Character* pC = static_cast<Character*>(pGO->GetComponent(TYPE_CHARACTER));
		if (pC == nullptr) { continue; }
		if (pC->mFriendly == false) {
			clearLevel = false;
			break;
		}
		else {
			playerAlive = true;
		}
	}

	if (!playerAlive) {
		mStage = 666;
		return;
	}

	if (nextLevel) {
		mStage++;
		if (mStage <= 4) {
			gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
		}
		else {
			mStage = 99;
		}
	}

	if (previousLevel)
	{
		if (mStage > 1)
		{
			mStage--;
			gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
		}
	}
	*/
}


