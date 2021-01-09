/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectFactory.h
Purpose: Handler of all object-type creations
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once

class GameObject;

class ObjectFactory {
public:
	ObjectFactory();
	~ObjectFactory();

	void RegisterCreator(const char* type, bool a);

	GameObject* LoadGameObject(const char* pFileName);
	GameObject* CreateGameObject();
	void LoadLevel(const char* pFileName);
public:


private:
private:
};
