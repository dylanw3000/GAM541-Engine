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
#include "..\FrameRateController.h"

extern ResourceManager* gpResourceManager;
extern FrameRateController* gpFRC;

Sprite::Sprite() : Component(TYPE_SPRITE) {
	mpSurface = nullptr;
}

Sprite::~Sprite() {
	//
}

void Sprite::Update() {
	//
	if (mIsAnimated)
		mSpriteAnimator->Update();
}

void Sprite::Serialize(std::ifstream& InputStream) {
	std::string imageName;
	InputStream >> imageName;

	imageName = "..\\Resources\\" + imageName;
	mpSurface = gpResourceManager->LoadSurface(imageName.c_str());
}

void Sprite::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	
	
	mIsAnimated = false;
	if (input[0].HasMember("animated")) {
		mIsAnimated = input[0]["animated"].GetBool();
		mSpriteAnimator = new SpriteAnimator();
	}
	
	
	if (mIsAnimated)
	{
		if (input[0].HasMember("columns")) {
			mColumns = input[0]["columns"].GetInt();
		}
		if (input[0].HasMember("rows")) {
			mRows = input[0]["rows"].GetInt();
		}

		for (int i = 0; i < mColumns; i++)
		{
			for (int j = 0; j < mRows; j++)
			{
				mSpriteAnimator->AddFrame(i * (1.0f/mColumns), j * (1.0f/mRows), 0.1f);
			}
		}

	}

	std::string imageName = input[0]["imageName"].GetString();
	mTexture = gpResourceManager->LoadTexture(("..\\Resources\\" + imageName).c_str(), &mWidth, &mHeight);
	

	// mpSurface = gpResourceManager->LoadSurface(("..\\Resources\\" + imageName).c_str());
	// mpSurface = gpResourceManager->LoadSurface("..\\Resources\\Angry.bmp");
}




SpriteAnimatorFrame::SpriteAnimatorFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mTextureOffsetX = TextureOffsetX;
	mTextureOffsetY = TextureOffsetY;
	mDuration = Duration;
}

SpriteAnimator::SpriteAnimator()
{
	mCurrentIndex = 0;
	mTimer = 0.0f;
}
SpriteAnimator::~SpriteAnimator()
{
	for (auto pFrame : mFrames)
		delete pFrame;
	mFrames.clear();
}

void SpriteAnimator::AddFrame(float TextureOffsetX, float TextureOffsetY, float Duration)
{
	mFrames.push_back(new SpriteAnimatorFrame(TextureOffsetX, TextureOffsetY, Duration));
}

void SpriteAnimator::Update()
{
	mTimer -= ((float)gpFRC->GetFrameTime() / 1000);
	if (mTimer <= 0) {
		++mCurrentIndex;
		mCurrentIndex %= mFrames.size();
		mTimer = mFrames[mCurrentIndex]->mDuration;
	}
}

std::pair<float, float> SpriteAnimator::GetTextureCoords()
{
	return std::pair<float, float>(mFrames[mCurrentIndex]->mTextureOffsetX, mFrames[mCurrentIndex]->mTextureOffsetY);
}