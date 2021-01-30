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
class Sprite;
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

	void AddRunningFrame(float TextureOffsetX, float TextureOffsetY, float Duration);
	void AddJumpingFrame(float TextureOffsetX, float TextureOffsetY, float Duration);
	void AddIdlingFrame(float TextureOffsetX, float TextureOffsetY, float Duration);
	void AddDashingFrame(float TextureOffsetX, float TextureOffsetY, float Duration);
	void AddAttackingFrame(float TextureOffsetX, float TextureOffsetY, float Duration);
	void AddFallingFrame(float TextureOffsetX, float TextureOffsetY, float Duration);

	void StartRunning();
	void StartJumping();
	void StartIdling();
	void StartDashing();
	void StartAttacking();
	void StartFalling();

	std::pair<float, float> GetTextureCoords();

public:
	Sprite* mParentSprite;

	unsigned int mRunningRows;
	unsigned int mRunningColumns;
	unsigned int mRunningTexture;
	bool mIsRunning;

	unsigned int mJumpingRows;
	unsigned int mJumpingColumns;
	unsigned int mJumpingTexture;
	bool mIsJumping;

	unsigned int mIdlingRows;
	unsigned int mIdlingColumns;
	unsigned int mIdlingTexture;
	bool mIsIdling;

	unsigned int mDashingRows;
	unsigned int mDashingColumns;
	unsigned int mDashingTexture;
	bool mIsDashing;

	unsigned int mAttackingRows;
	unsigned int mAttackingColumns;
	unsigned int mAttackingTexture;
	bool mIsAttacking;

	unsigned int mFallingRows;
	unsigned int mFallingColumns;
	unsigned int mFallingTexture;
	bool mIsFalling;

private:
	std::vector<SpriteAnimatorFrame*> mFrames;
	size_t mCurrentIndex;
	float mTimer;


	std::vector<SpriteAnimatorFrame*> mRunningFrames;
	std::vector<SpriteAnimatorFrame*> mJumpingFrames;
	std::vector<SpriteAnimatorFrame*> mIdlingFrames;
	std::vector<SpriteAnimatorFrame*> mDashingFrames;
	std::vector<SpriteAnimatorFrame*> mAttackingFrames;
	std::vector<SpriteAnimatorFrame*> mFallingFrames;



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


	SpriteAnimator* mpSpriteAnimator;




private:
private:

};



