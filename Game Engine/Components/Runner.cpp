
#include "Runner.h"
#include "Transform.h"
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

extern ObjectFactory* gpObjectFactory;
extern ResourceManager* gpResourceManager;
extern GameObjectManager* gpGameObjectManager;
extern FrameRateController* gpFRC;
extern InputManager* gpInputManager;

Runner::Runner() : Component(TYPE_RUNNER) {
	mTimer = 0;
	mGravity = 1200.f;
	mAcc = 1200.f;

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
}

Runner::~Runner() {
	//
}

void Runner::Update() {
	float delta = gpFRC->GetDeltaTime();

	/*** Upkeep ***/
	mTimer += delta;

	for (auto pGO : gpGameObjectManager->mGameObjects) {
		Transform* pT = static_cast<Transform*>(pGO->GetComponent(TYPE_TRANSFORM));
		Sprite* pS = static_cast<Sprite*>(pGO->GetComponent(TYPE_SPRITE));
		if (pT->mPositionX < 0.f) {
			pGO->mDestroy = true;
		}
	}

	while (mTimer > 0.3) {
		mTimer -= 0.3;
		gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");

		GameObject* pWall = gpObjectFactory->LoadGameObject("../Resources/RunnerWall.json");
		Transform* pT = static_cast<Transform*>(pWall->GetComponent(TYPE_TRANSFORM));
		pT->mPositionY = 800;
	}


	/*** Player Behaviour ***/
	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	Body* pB = static_cast<Body*>(mpOwner->GetComponent(TYPE_BODY));

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

void Runner::Serialize(rapidjson::GenericArray<false, rapidjson::Value> input) {
	if (input[0].HasMember("sample")) {
		// mHeight = input[0]["height"].GetFloat();
	}
}
