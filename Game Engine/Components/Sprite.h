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
#include <vector>

class GameObject;
struct SDL_Surface;


class SpriteAnimatorFrame
{

public:
	SpriteAnimatorFrame(float TextureOffsetX, float TextureOffsetY, float Duration);
public:
	float mTextureOffsetX, mTextureOffsetY;
	float mDuration;

};

class SpriteAnimator
{
public:
	SpriteAnimator();
	~SpriteAnimator();
	void Update();
	void AddFrame(float TextureOffsetX, float TextureOffsetY, float Duration);
	std::pair<float, float> GetTextureCoords();

private:
	std::vector<SpriteAnimatorFrame*> mFrames;
	size_t mCurrentIndex;
	float mTimer;
};

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
	bool mIsAnimated;

	unsigned int mRows;
	unsigned int mColumns;
	int mWidth;
	int mHeight;

	SpriteAnimator* mSpriteAnimator;




private:
private:

};



