
#include "Runner.h"
#include "Transform.h"
#include "Character.h"
#include "Sprite.h"
#include "Body.h"
#include "../InputManager.h"
#include "../GameObject.h"
#include "../EventManager.h"
#include "../GameObjectManager.h"
#include "../ObjectFactory.h"
#include "../ResourceManager.h"
#include "../GameObjectManager.h"
#include "../FrameRateController.h"
#include <stdlib.h>
#include <time.h>

extern ObjectFactory* gpObjectFactory;
extern ResourceManager* gpResourceManager;
extern GameObjectManager* gpGameObjectManager;
extern FrameRateController* gpFRC;
extern InputManager* gpInputManager;

extern bool DEBUG;

Runner::Runner() : Component(TYPE_RUNNER) {
	mTimer = 0;
	mGravity = 1600.f;
	mAcc = 2400.f;

	mWallTime = mWallDelay = 0.2;
	mObstacleTime = mObstacleDelay = 1.6;
	mMissileTime = mMissileDelay = 6.f;

	mMissileActive = false;
	mMissileLaunchTime = mMissileLaunchDelay = 2.f;

	mFail = false;

	// gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
	
	for (int i = 0; i <= 1264 / 64; i++) {
		GameObject* pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
		Transform* pT = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
		pT->mPositionX = i * 64;

		pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
		pT = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
		pT->mPositionX = i * 64;
		pT->mPositionY = 800;
	}

	srand(time(NULL));
}

Runner::~Runner() {
	//
}

void Runner::Update() {
	float delta = gpFRC->GetDeltaTime();

	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	Body* pB = static_cast<Body*>(mpOwner->GetComponent(TYPE_BODY));
	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));

	if (mFail) {
		for (auto pGO : gpGameObjectManager->mGameObjects) {
			Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
			pTrans->mVelHoriz = 0.f;
			pTrans->mVelVert = 0.f;
		}

		pC->AddTelegraph(pT->mPositionX, pT->mPositionY - pB->mHeight / 2.f, 0, M_PI, 128, 1200, 0.f);
	}
	else {
		/*** Upkeep ***/
		mTimer += delta;

		for (auto pGO : gpGameObjectManager->mGameObjects) {
			if (pGO == mpOwner) { continue; }

			Transform* pTrans = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
			if (pTrans->mPositionX < 0.f) {
				pGO->mDestroy = true;
				continue;
			}

			Body* pBody = static_cast<Body*>(pGO->GetComponent(TYPE_BODY));

			if (
				pBody != nullptr
				&& pT->mPositionX - pB->mWidth / 2.f < pTrans->mPositionX + pBody->mWidth / 2.f
				&& pT->mPositionX + pB->mWidth / 2.f > pTrans->mPositionX - pBody->mWidth / 2.f
				&& pT->mPositionY - pB->mHeight < pTrans->mPositionY
				&& pT->mPositionY > pTrans->mPositionY - pBody->mHeight
			) {
				mFail = true;
			}
		}

		/*** (De)Spawning ***/
		if (mTimer > mWallTime) {
			mWallTime += mWallDelay;

			gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");

			GameObject* pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
			Transform* pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
			pTrans->mPositionY = 800;
		}

		if (mTimer > mObstacleTime) {
			mObstacleTime += mObstacleDelay;

			GameObject* pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerObstacle.json");
			Transform* pTrans = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
			pTrans->mPositionY = rand() % 414 + 320;
		}

		/*** Missiles ***/
		if (mTimer > mMissileTime) {
			mMissileTime += mMissileDelay;
			mMissileActive = true;
			mMissileLaunchTime = 0.f;
		}

		if (mMissileActive) {
			mMissileLaunchTime += delta;
			pC->AddRect(1200.f, pT->mPositionY - pB->mHeight / 2.f, M_PI, 1200.f, 32.f, mMissileLaunchTime / mMissileLaunchDelay);

			if (mMissileLaunchTime >= mMissileLaunchDelay) {
				mMissileActive = false;
				GameObject* pMissile = gpObjectFactory->LoadGameObject("../Resources/RunnerMissile.json");
				Transform* pTrans = static_cast<Transform*>(pMissile->GetComponent(TYPE_TRANSFORM));
				pTrans->mPositionY = pT->mPositionY - pB->mHeight / 2.f + 8.f;
			}
		}

		/*** Player Behaviour ***/
		if (gpInputManager->IsKeyPressed(SDL_SCANCODE_W) || gpInputManager->IsKeyPressed(SDL_SCANCODE_SPACE)) {
			pT->mVelVert -= mAcc * delta;
		}
		else {
			pT->mVelVert += mGravity * delta;
		}

		if (pT->mVelVert < 0.f) {
			if (pT->mPositionY - pB->mHeight < 64.f) {
				pT->mPositionY = 64.f + pB->mHeight;
				pT->mVelVert = 0.f;
			}
		}
		else {
			if (pT->mPositionY > 736.f) {
				pT->mPositionY = 736.f;
				pT->mVelVert = 0.f;
			}
		}
	}


}

void Runner::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	if (input[0].HasMember("sample")) {
		// mHeight = input[0]["height"].GetFloat();
	}
}
