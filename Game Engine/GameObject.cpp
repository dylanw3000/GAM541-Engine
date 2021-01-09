/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GameObject.cpp
Purpose: Creates objects and handles collection of components
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "GameObject.h"
#include "EventManager.h"

#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Components/Character.h"
#include "Components/Controller.h"
#include "Components/UpDown.h"
#include "Components/Component.h"
#include "Components/Slime.h"
#include "Components/Augmentor.h"
#include "Components/Sniper.h"

GameObject::GameObject() {
	//
}

GameObject::~GameObject() {
	for (auto pComponent : mComponents)
		delete pComponent;
	mComponents.clear();
}

void GameObject::Update() {
	for (auto pComponent : mComponents)
		pComponent->Update();
}

Component* GameObject::AddComponent(unsigned int type) {
	Component* pNewComponent;
	pNewComponent = GetComponent(type);
	if (pNewComponent == nullptr) {
		switch (type) {
		case TYPE_TRANSFORM:
			pNewComponent = new Transform();
			break;
		case TYPE_SPRITE:
			pNewComponent = new Sprite();
			break;
		case TYPE_CHARACTER:
			pNewComponent = new Character();
			break;
		case TYPE_PLAYER_CONTROLLER:
			pNewComponent = new Controller();
			break;
		case TYPE_UP_DOWN:
			pNewComponent = new UpDown();
			break;
		case TYPE_SLIME:
			pNewComponent = new Slime();
			break;
		case TYPE_AUGMENTOR:
			pNewComponent = new Augmentor();
			break;
		case TYPE_SNIPER:
			pNewComponent = new Sniper();
			break;
		default:
			pNewComponent = nullptr;
		}
	}

	if (nullptr != pNewComponent) {
		mComponents.push_back(pNewComponent);
		pNewComponent->mpOwner = this;
	}

	return pNewComponent;
}

Component* GameObject::GetComponent(unsigned int type) {
	for (auto pComponent : mComponents)
		if (pComponent->GetType() == type)
			return pComponent;

	return nullptr;
}

void GameObject::HandleEvent(Event *pEvent) {
	for (auto pComponent : mComponents)
		pComponent->HandleEvent(pEvent);
}
