/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.cpp
Purpose: Contains all sprite information for an object
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "Sprite.h"
#include "..\ResourceManager.h"
#include "SDL_surface.h"

extern ResourceManager* gpResourceManager;

Sprite::Sprite() : Component(TYPE_SPRITE) {
	mpSurface = nullptr;
}

Sprite::~Sprite() {
	//
}

void Sprite::Update() {
	//
}

void Sprite::Serialize(std::ifstream& InputStream) {
	std::string imageName;
	InputStream >> imageName;

	imageName = "..\\Resources\\" + imageName;
	mpSurface = gpResourceManager->LoadSurface(imageName.c_str());
}

void Sprite::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	std::string imageName = input[0].GetString();
	mTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str());
	// mpSurface = gpResourceManager->LoadSurface(("..\\Resources\\" + imageName).c_str());
	// mpSurface = gpResourceManager->LoadSurface("..\\Resources\\Angry.bmp");
}
