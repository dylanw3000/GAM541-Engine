/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObjectManager.h
Purpose: Handler of all object-type creations
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include <vector>

class GameObject;

class GameObjectManager {
public:
	GameObjectManager();
	~GameObjectManager();

public:
	std::vector<GameObject*> mGameObjects;

	void DeleteObject(GameObject* pGO);
	bool mDestroy;


private:
private:
};
