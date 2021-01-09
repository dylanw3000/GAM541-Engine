/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.h
Purpose: Loads and maintains resources for use
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_1
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 5, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include <unordered_map>
// #include <gl/GL.h>
// #include "SDL_surface.h"

struct SDL_Surface;


class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	
	SDL_Surface* LoadSurface(const char* pFilePath);
	unsigned int LoadTexture(const char* pFilePath);
public:


private:
private:

	std::unordered_map<std::string, SDL_Surface*> mSurfaces;
	std::unordered_map<std::string, unsigned int> mTextures;
};
