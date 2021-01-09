/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.h
Purpose: Creates objects and handles collection of components
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include <vector>

/*
class Transform;
class Sprite;
class Controller;
class UpDown;
*/
class Component;
class Event;

class GameObject {
public:
	GameObject();
	~GameObject();

	void Update();
	Component* AddComponent(unsigned int type);
	Component* GetComponent(unsigned int type);

	void HandleEvent(Event* pEvent);

public:
	std::vector<Component*> mComponents;


private:
private:
};
