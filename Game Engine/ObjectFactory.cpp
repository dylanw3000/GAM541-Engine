/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ObjectFactory.cpp
Purpose: Handler of all object-type creations
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "ObjectFactory.h"

#include <string>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

#include "GameObjectManager.h"
#include "GameObject.h"

#include "Components/Component.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/Character.h"
#include "Components/Controller.h"
#include "Components/UpDown.h"
#include "Components/Slime.h"
#include "Components/Augmentor.h"
#include "Components/Sniper.h"
#include "Components/AudioClip.h"
#include "Components/Body.h"
#include "Components/Runner.h"
#include "Components/LeftRight.h"
#include "Components/Objective.h"

extern GameObjectManager* gpGameObjectManager;

ObjectFactory::ObjectFactory() {
	//
}

ObjectFactory::~ObjectFactory() {
	//
}


void ObjectFactory::RegisterCreator(const char* type, bool a) {
	//
}


GameObject* ObjectFactory::CreateGameObject() {
	GameObject* pNewGO = new GameObject();
	gpGameObjectManager->mGameObjects.push_back(pNewGO);
	return pNewGO;
}

GameObject* ObjectFactory::LoadGameObject(const char* pFileName) {
	GameObject* pNewGO = nullptr;
	// std::string componentName;
	// std::ifstream inputStream(pFileName);

	std::ifstream ifs(pFileName);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);
	ifs.close();

	Component* pNewComponent = nullptr;

	pNewGO = new GameObject();
	// for (auto& d : document.GetObject()) {
		// Component* pNewComponent = nullptr;

	if (document.HasMember("transform")) {
		pNewComponent = pNewGO->AddComponent(TYPE_TRANSFORM);
		pNewComponent->Serialize(document["transform"].GetArray());
	}
	if (document.HasMember("sprite")) {
		pNewComponent = pNewGO->AddComponent(TYPE_SPRITE);
		pNewComponent->Serialize(document["sprite"].GetArray());
	}

	if (document.HasMember("character")) {
		pNewComponent = pNewGO->AddComponent(TYPE_CHARACTER);
		pNewComponent->Serialize(document["character"].GetArray());
	}
	if (document.HasMember("controller")) {
		pNewComponent = pNewGO->AddComponent(TYPE_PLAYER_CONTROLLER);
		pNewComponent->Serialize(document["controller"].GetArray());
	}
	if (document.HasMember("updown")) {
		pNewComponent = pNewGO->AddComponent(TYPE_UP_DOWN);
		pNewComponent->Serialize(document["updown"].GetArray());
	}
	if (document.HasMember("slime")) {
		pNewComponent = pNewGO->AddComponent(TYPE_SLIME);
		pNewComponent->Serialize(document["slime"].GetArray());
	}
	if (document.HasMember("augmentor")) {
		pNewComponent = pNewGO->AddComponent(TYPE_AUGMENTOR);
		pNewComponent->Serialize(document["augmentor"].GetArray());
	}
	if (document.HasMember("sniper")) {
		pNewComponent = pNewGO->AddComponent(TYPE_SNIPER);
		pNewComponent->Serialize(document["sniper"].GetArray());
	}
	if (document.HasMember("body")) {
		pNewComponent = pNewGO->AddComponent(TYPE_BODY);
		pNewComponent->Serialize(document["body"].GetArray());
	}
	if (document.HasMember("audioClip")) {
		pNewComponent = pNewGO->AddComponent(TYPE_AUDIOCLIP);
		pNewComponent->Serialize(document["audioClip"].GetArray());
	}
	if (document.HasMember("runner")) {
		pNewComponent = pNewGO->AddComponent(TYPE_RUNNER);
		pNewComponent->Serialize(document["runner"].GetArray());
	}
	if (document.HasMember("leftright")) {
		pNewComponent = pNewGO->AddComponent(TYPE_LEFT_RIGHT);
		pNewComponent->Serialize(document["leftright"].GetArray());
	}
	if (document.HasMember("objective")) {
		pNewComponent = pNewGO->AddComponent(TYPE_OBJECTIVE);
		pNewComponent->Serialize(document["objective"].GetArray());
	}

	gpGameObjectManager->mGameObjects.push_back(pNewGO);

	return pNewGO;
}

void ObjectFactory::LoadLevel(const char* pFileName) {

	std::ifstream ifs(pFileName);
	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);
	ifs.close();

	assert(document.HasMember("objects"));
	assert(document["objects"].IsArray());

	// for (int i = 0; i < document["objects"].Size(); i++) {
	for (auto& d : document["objects"].GetArray()) {
		GameObject* pNewGO;
		if (d.HasMember("name")) {
			std::string path = d["name"].GetString();
			pNewGO = LoadGameObject(("..\\Resources\\" + path).c_str());
		}
		else {
			pNewGO = CreateGameObject();
		}

		if (nullptr == pNewGO) {
			continue;
		}

		

		if (d.HasMember("transform")) {		// TODO "components": { "transform": [ 1, 2 ], "sprite": "Angry.bmp" }
			Transform* pTransform = static_cast<Transform*>(pNewGO->GetComponent(TYPE_TRANSFORM));
			if(pTransform == nullptr){ pTransform = static_cast<Transform*>(pNewGO->AddComponent(TYPE_TRANSFORM)); }
			assert(pTransform != nullptr);
			pTransform->Serialize(d["transform"].GetArray());
		}

		if (d.HasMember("sprite")) {
			Sprite* pSprite = static_cast<Sprite*>(pNewGO->GetComponent(TYPE_SPRITE));
			if (pSprite == nullptr) { pSprite = static_cast<Sprite*>(pNewGO->AddComponent(TYPE_SPRITE)); }
			assert(pSprite != nullptr);
			pSprite->Serialize(d["sprite"].GetArray());
		}
		
		if (d.HasMember("character")) {
			Character* pCharacter = static_cast<Character*>(pNewGO->GetComponent(TYPE_CHARACTER));
			if (pCharacter != nullptr) {
				pCharacter->Serialize(d["character"].GetArray());
			}
			else {
				pCharacter = static_cast<Character*>(pNewGO->AddComponent(TYPE_CHARACTER));
				pCharacter->Serialize(d["character"].GetArray());
			}
		}

		if (d.HasMember("controller")) {
			Controller* pController = static_cast<Controller*>(pNewGO->GetComponent(TYPE_PLAYER_CONTROLLER));
			assert(pController != nullptr);
			pController->Serialize(d["controller"].GetArray());
		}

		if (d.HasMember("updown")) {
			UpDown* pUpDown = static_cast<UpDown*>(pNewGO->GetComponent(TYPE_UP_DOWN));
			assert(pUpDown != nullptr);
			pUpDown->Serialize(d["updown"].GetArray());
		}

		if (d.HasMember("leftright")) {
			LeftRight* pLeftRight = static_cast<LeftRight*>(pNewGO->GetComponent(TYPE_LEFT_RIGHT));
			assert(pLeftRight != nullptr);
			pLeftRight->Serialize(d["leftright"].GetArray());
		}

		if (d.HasMember("slime")) {
			Slime* pSlime = static_cast<Slime*>(pNewGO->GetComponent(TYPE_SLIME));
			assert(pSlime != nullptr);
			pSlime->Serialize(d["slime"].GetArray());
		}
		
		if (d.HasMember("augmentor")) {
			Augmentor* pAugmentor = static_cast<Augmentor*>(pNewGO->GetComponent(TYPE_AUGMENTOR));
			assert(pAugmentor != nullptr);
			pAugmentor->Serialize(d["augmentor"].GetArray());
		}
		
		if (d.HasMember("sniper")) {
			Sniper* pSniper = static_cast<Sniper*>(pNewGO->GetComponent(TYPE_SNIPER));
			assert(pSniper != nullptr);
			pSniper->Serialize(d["sniper"].GetArray());
		}

		if (d.HasMember("audioClip")) {
			AudioClip* pAudioClip = static_cast<AudioClip*>(pNewGO->GetComponent(TYPE_AUDIOCLIP));
			assert(pAudioClip != nullptr);
			pAudioClip->Serialize(d["audioClip"].GetArray());
		}

		
		if (d.HasMember("body")) {
			Body* pBody = static_cast<Body*>(pNewGO->GetComponent(TYPE_BODY));
			if (pBody == nullptr) { pBody = static_cast<Body*>(pNewGO->AddComponent(TYPE_BODY)); }
			assert(pBody != nullptr);
			pBody->Serialize(d["body"].GetArray());
		}

		if (d.HasMember("runner")) {
			Runner* pRunner = static_cast<Runner*>(pNewGO->GetComponent(TYPE_RUNNER));
			if (pRunner == nullptr) { pRunner = static_cast<Runner*>(pNewGO->AddComponent(TYPE_RUNNER)); }
			pRunner->Serialize(d["runner"].GetArray());
		}

		if (d.HasMember("objective")) {
			Objective* pObjective = static_cast<Objective*>(pNewGO->GetComponent(TYPE_OBJECTIVE));
			if (pObjective == nullptr) { pObjective = static_cast<Objective*>(pNewGO->AddComponent(TYPE_OBJECTIVE)); }
			pObjective->Serialize(d["objective"].GetArray());
		}
		
	}

	/*
	std::string objectName;
	std::ifstream inputStream(pFileName);

	if (inputStream.is_open()) {
		while (getline(inputStream, objectName)) {
			GameObject* pNewGO = LoadGameObject(("..\\Resources\\" + objectName).c_str());

			if (nullptr != pNewGO) {
				Component* pGOComponent = pNewGO->GetComponent(TYPE_TRANSFORM);
				pGOComponent->Serialize(inputStream);
			}
		}

		inputStream.close();
	}
	*/
}
