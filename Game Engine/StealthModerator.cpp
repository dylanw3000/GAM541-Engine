#include "StealthModerator.h"
#include <string>

#include "GameObject.h"
#include "Components/Component.h"
#include "Components/Character.h"

#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "ObjectFactory.h"

extern ResourceManager* gpResourceManager;
extern GameObjectManager* gpGameObjectManager;
extern ObjectFactory* gpObjectFactory;

extern int gameType;

StealthModerator::StealthModerator() {
	mStage = 0;
	mTimer = 0.0f;
	mTransition = true;
}

StealthModerator::~StealthModerator() {
	//
}

void StealthModerator::Update() {
	if (gameType == 2 || mStage == 0 || mStage == 99 || mStage == 666) {
		return;
	}

	bool clearLevel = false;
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

	if (clearLevel) {
		mStage++;
		if (mStage <= 4) {
			gpObjectFactory->LoadLevel(("..\\Resources\\StealthLevel" + std::to_string(mStage) + ".json").c_str());
		}
		else {
			mStage = 99;
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


