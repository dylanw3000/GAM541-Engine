/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.h
Purpose: Contains all sprite information for an object
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once
#include "Component.h"

class GameObject;
struct SDL_Surface;

class Sprite : public Component {
public:
	Sprite();
	~Sprite();

	void Update();
	void Serialize(std::ifstream& InputStream);
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

public:
	SDL_Surface* mpSurface;
	unsigned int mTexture;


private:
private:

};
