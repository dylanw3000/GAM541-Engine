/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.cpp
Purpose: Handler of all object-type creations
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "GameObjectManager.h"
#include "GameObject.h"
#include <vector>
#include <algorithm>

GameObjectManager::GameObjectManager() {
	mDestroy = false;
}

GameObjectManager::~GameObjectManager() {
	for (auto pGO : mGameObjects) {
		pGO->~GameObject();
		delete pGO;
	}

	mGameObjects.clear();
}

void GameObjectManager::DeleteObject(GameObject* pGO) {
	if (!mDestroy) {
		mDestroy = true;
		std::vector<GameObject*>::iterator it;
		it = std::find(mGameObjects.begin(), mGameObjects.end(), pGO);
		mGameObjects.erase(it);
	}
}
