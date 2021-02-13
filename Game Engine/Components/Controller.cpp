/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Controller.cpp
Purpose: Handles player movement
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#include "Controller.h"
#include "Sprite.h"
#include "..\InputManager.h"
#include "..\..\SDL2.0 Lib\include\SDL_scancode.h"
#include "..\GameObject.h"
#include "Transform.h"
#include "Character.h"
#include "Body.h"
#include "math.h"
#include "..\FrameRateController.h"
#include "../GameObjectManager.h"
#include "../EventManager.h"
#include "../CollisionManager.h"

extern FrameRateController* gpFRC;
extern InputManager* gpInputManager;
extern GameObjectManager* gpGameObjectManager;
extern EventManager* gpEventManager;


PlayerHitEvent::PlayerHitEvent() : Event(EventType::PLAYER_HIT) {
	//
}

PlayerHitEvent::~PlayerHitEvent() {
	//
}

/**************************************/

Controller::Controller() : Component(TYPE_PLAYER_CONTROLLER) {
	// mVelHoriz = 0.0f;
	// mVelVert = 0.0f;

	mAcceleration = 1600;
	mMaxSpeed = 160;

	mSlowMod = 0.25;
	mExcessSlowMod = 2.0f;

	mDashTimer = mDashCooldown = 2000;


	mSwingTime = 800;
	mSwingDelay = 400;
	mSwingTimer = mSwingTime + mSwingDelay;
	mSwingAng = 0.0f;
	mSwingWidth = 0.9f;
	mSwinging = mCleaver = false;
}

Controller::~Controller() {
	//
}

extern bool DEBUG;

void Controller::Update() {

	Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));

	float h_mod, v_mod;
	// h_mod = ((gpInputManager->IsKeyPressed(SDL_SCANCODE_RIGHT) || gpInputManager->IsKeyPressed(SDL_SCANCODE_D)) - (gpInputManager->IsKeyPressed(SDL_SCANCODE_LEFT) || gpInputManager->IsKeyPressed(SDL_SCANCODE_A)));
	//8 v_mod = ((gpInputManager->IsKeyPressed(SDL_SCANCODE_DOWN) || gpInputManager->IsKeyPressed(SDL_SCANCODE_S)) - (gpInputManager->IsKeyPressed(SDL_SCANCODE_UP) || gpInputManager->IsKeyPressed(SDL_SCANCODE_W)));

	h_mod = (gpInputManager->IsKeyPressed(SDL_SCANCODE_D) - gpInputManager->IsKeyPressed(SDL_SCANCODE_A));
	v_mod = (gpInputManager->IsKeyPressed(SDL_SCANCODE_S) - gpInputManager->IsKeyPressed(SDL_SCANCODE_W));
	if (h_mod != 0 && v_mod != 0) {
		h_mod *= 0.7;
		v_mod *= 0.7;
	}

	mDashTimer += gpFRC->GetFrameTime();
	if (gpInputManager->IsKeyTriggered(SDL_SCANCODE_SPACE)) {
		/*
		mDashTimer = 0;

		if (h_mod == 0 && v_mod == 0) {
			pT->mVelHoriz += 1000;
		}
		else {
			pT->mVelHoriz += 1000 * h_mod;
			pT->mVelVert += 1000 * v_mod;
		}
		*/
		pT->mVelVert = -600; // jumping
	}

	if ((gpInputManager->IsKeyPressed(SDL_SCANCODE_A) || gpInputManager->IsKeyPressed(SDL_SCANCODE_D)) && gpInputManager->IsKeyPressed(SDL_SCANCODE_LCTRL) && mDashTimer >= mDashCooldown)
	{

		mDashTimer = 0;
		//if already triggered within the Limit
		if (h_mod == 0 && v_mod == 0) {
			pT->mVelHoriz += 1000;
		}
		else {
			pT->mVelHoriz += 1000 * h_mod;
			pT->mVelVert += 1000 * v_mod;
		}
		

		
	}

	float dTime = gpFRC->GetDeltaTime();
	


	Body* pB = static_cast<Body*>(mpOwner->GetComponent(TYPE_BODY));
	/*** Vert ***/
	// pT->mVelVert += mAcceleration * v_mod * dTime;
	pT->mVelVert += 800 * dTime;  // gravity

	{
		bool collision = false;
		float pos = 0.0;

		for (auto pObject : gpGameObjectManager->mGameObjects) {
			Body* pBody = static_cast<Body*>(pObject->GetComponent(TYPE_BODY));
			if (!pBody || pBody->mpOwner == mpOwner) { continue; }

			Transform* pTrans = static_cast<Transform*>(pObject->GetComponent(TYPE_TRANSFORM));

			if (
				pT->mPositionY + (pT->mVelVert * dTime) >= pTrans->mPositionY - pBody->mHeight
				&& pT->mPositionY - pB->mHeight + (pT->mVelVert * dTime) <= pTrans->mPositionY
				&& pT->mPositionX - pB->mWidth / 2 <= pTrans->mPositionX + pBody->mWidth / 2
				&& pT->mPositionX + pB->mWidth / 2 >= pTrans->mPositionX - pBody->mWidth / 2
				) {
				if (!collision) {
					collision = true;
					pos = pTrans->mPositionY + (pT->mVelVert >= 0 ? -pBody->mHeight - .001 : pB->mHeight + .001);
				}
				// pT->mVelVert = 0.0f;
			}
		}

		if (collision) {
			pT->mVelVert = 0.0f;
			pT->mPositionY = pos;
		}

	}


	/*** Horiz ***/
	pT->mVelHoriz += mAcceleration * h_mod * dTime;
	{
		bool collision = false;
		float pos = 0.0;


		for (auto pObject : gpGameObjectManager->mGameObjects) {
			Body* pBody = static_cast<Body*>(pObject->GetComponent(TYPE_BODY));
			if (!pBody || pBody->mpOwner == mpOwner) { continue; }

			Transform* pTrans = static_cast<Transform*>(pObject->GetComponent(TYPE_TRANSFORM));

			if (
				pT->mPositionY >= pTrans->mPositionY - pBody->mHeight
				&& pT->mPositionY - pB->mHeight <= pTrans->mPositionY
				&& pT->mPositionX - pB->mWidth / 2 + (pT->mVelHoriz * dTime) <= pTrans->mPositionX + pBody->mWidth / 2
				&& pT->mPositionX + pB->mWidth / 2 + (pT->mVelHoriz * dTime) >= pTrans->mPositionX - pBody->mWidth / 2
				) {
				
				if (!collision) {
					collision = true;
					pos = pTrans->mPositionX + (pB->mWidth/2 + pBody->mWidth/2 + .001) * (pT->mVelHoriz >= 0 ? -1 : 1);
				}
				
				// pT->mVelHoriz = 0.0f;
			}
		}

		if (collision) {
			pT->mVelHoriz = 0.0f;
			pT->mPositionX = pos;
		}

	}


	/*** Slowing ***/
	float ang = atan2f(pT->mVelVert, pT->mVelHoriz);
	if (pT->mVelHoriz != 0) {
		if (fabsf(pT->mVelHoriz) - fabsf(mAcceleration * cosf(ang) * mSlowMod * dTime) <= 0) {
			pT->mVelHoriz = 0;
		}
		else {
			pT->mVelHoriz -= mAcceleration * cosf(ang) * mSlowMod * dTime;
		}
	}

	/*
	if (pT->mVelVert != 0) {
		if (fabsf(pT->mVelVert) - fabsf(mAcceleration * sinf(ang) * mSlowMod * dTime) <= 0) {
			pT->mVelVert = 0;
		}
		else {
			pT->mVelVert -= mAcceleration * sinf(ang) * mSlowMod * dTime;
		}
	}
	*/
	

	// TODO: Strip out code from when game was top-down
	// float sqVel = (pT->mVelHoriz * pT->mVelHoriz + pT->mVelVert * pT->mVelVert) / (mMaxSpeed * mMaxSpeed);
	float sqVel = (pT->mVelHoriz * pT->mVelHoriz) / (mMaxSpeed * mMaxSpeed);
	if (sqVel > 1.0f) {
		// ang = atan2f(mVelVert, mVelHoriz);
		if(fabsf(pT->mVelHoriz) - mAcceleration*mExcessSlowMod*dTime <= mMaxSpeed){
		// if (sqrtf(pT->mVelHoriz * pT->mVelHoriz + pT->mVelVert * pT->mVelVert) - mAcceleration * mExcessSlowMod * dTime <= mMaxSpeed) {
			/*
			pT->mVelHoriz /= sqVel;
			pT->mVelVert /= sqVel;
			*/
			pT->mVelHoriz /= sqVel;
		}
		else {
			pT->mVelHoriz -= mAcceleration * cosf(ang) * mExcessSlowMod * dTime;
			/*
			pT->mVelHoriz -= mAcceleration * cosf(ang) * mExcessSlowMod * dTime;
			pT->mVelVert -= mAcceleration * sinf(ang) * mExcessSlowMod * dTime;
			*/
		}
	}

	/*
	if (sqVel > 1.0f) {
		mVelHoriz /= sqVel;
		mVelVert /= sqVel;
	}
	*/


	/*** Final Position ***/
	// Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
	// pT->mPositionX += mVelHoriz * dTime;
	// pT->mPositionY += mVelVert * dTime;



	/*** Attacks ***/
	mSwingTimer += gpFRC->GetFrameTime();
	if ((gpInputManager->IsMousePressed() || (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))) && !mSwinging && mSwingTimer >= mSwingTime+mSwingDelay) {
		mSwinging = true;
		mSwingTimer = 0;
		mCleaver = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
	}

	Character* pC = static_cast<Character*>(mpOwner->GetComponent(TYPE_CHARACTER));
	Sprite* pS = static_cast<Sprite*>(mpOwner->GetComponent(TYPE_SPRITE));
	if (mSwinging) {
		if (pS->mIsAnimated)
		{
			if (!pS->mpSpriteAnimator->mIsAttacking)
				pS->mpSpriteAnimator->StartAttacking();
		}
		mSwingAng = atan2(gpInputManager->mMouseY - pT->mPositionY, gpInputManager->mMouseX - pT->mPositionX);
		// mSwingAng > 0.0 && mSwingAng < 1.0 ? pT->mSpriteOffsetY = -30 : pT->mSpriteOffsetY = 0;
		// mSwingAng + mSwingWidth > 0 ? pT->mSpriteOffsetX = -30 : pT->mSpriteOffsetX = 0;
		if (!mCleaver) {
			pC->AddTelegraphColor(pT->mPositionX, pT->mPositionY, mSwingAng, mSwingWidth, 20, 120, (float)mSwingTimer / mSwingTime, .2, .2, .8, .5, .3, .3, .8, .5);
		}
		else {
			pC->AddTelegraphColor(pT->mPositionX - 100*cosf(mSwingAng), pT->mPositionY - 100*sinf(mSwingAng), mSwingAng, mSwingWidth * 0.6, 120, 500, (float)mSwingTimer / mSwingTime, .2, .2, .8, .5, .3, .3, .8, .5);
			pC->mHP = pC->mHPMax;
		}

		if (mSwingTimer >= mSwingTime) {
			pS->mpSpriteAnimator->mIsAttacking = false;
			mSwinging = false;			
			// pC->mHP -= 1;

			for (auto pObject : gpGameObjectManager->mGameObjects) {
				Character* pChar = static_cast<Character*>(pObject->GetComponent(TYPE_CHARACTER));
				if (pChar == nullptr || pChar->mFriendly == true)
					continue;

				if (!mCleaver) {
					if (pChar->CollideCirc(pT->mPositionX, pT->mPositionY, mSwingAng, mSwingWidth, 0, 120)) {
						pChar->mHP -= 1.0f;
					}
				}
				else {
					if (pChar->CollideCirc(pT->mPositionX - 100 * cosf(mSwingAng), pT->mPositionY - 100 * sinf(mSwingAng), mSwingAng, mSwingWidth * 0.6, 120, 500)) {
						pChar->mHP -= 10.0f;
					}
					
				}
				

				/*
				Transform* pT = static_cast<Transform*>(pObject->GetComponent(TYPE_TRANSFORM));
				if (pT == nullptr)
					continue;

				float dist = powf(pT->mPositionX - pT->mPositionX, 2) + powf(pT->mPositionY - pT->mPositionY, 2);
				if (dist <= powf(120 + pChar->mRadius, 2)) {
					float ang = atan2f(pT->mPositionY - pT->mPositionY, pT->mPositionX - pT->mPositionX);
					float angMin = atan2f((pT->mPositionY + pChar->mRadius * sinf(ang - 3.14159 / 2)) - pT->mPositionY, (pT->mPositionX + pChar->mRadius * cosf(ang - 3.14159 / 2)) - pT->mPositionX);
					float angMax = atan2f((pT->mPositionY + pChar->mRadius * sinf(ang + 3.14159 / 2)) - pT->mPositionY, (pT->mPositionX + pChar->mRadius * cosf(ang + 3.14159 / 2)) - pT->mPositionX);

					float swingMin = mSwingAng - mSwingWidth;
					float swingMax = mSwingAng + mSwingWidth;

					if (dist < powf(pChar->mRadius, 2) || 
						(
							(angMin < swingMax && angMax > swingMin) 
							|| angMin < swingMax - 2 * 3.14159
							|| angMax > swingMin + 2 * 3.14159
							|| (angMax < angMin 
								&& (
									(angMin - 2*3.14159 < swingMax && angMax > swingMin)
									|| (angMin < swingMax && angMax + 2*3.14159 > swingMin)
								)
							)
						)
					) {
						pChar->mHP -= 1;
					}
					
				} */
			}
		}
	}

	if (mDashTimer < mDashCooldown)
		pC->AddRectColor(pT->mPositionX + 32.f, pT->mPositionY, -3.14159/2.0f, 50.f, 6.f, (float)mDashTimer / mDashCooldown, .4, .4, .8, 1.f, .2, .2, .8, 1.f);


	/*** Milestone 3 ***/
	pT->mAngle += (gpInputManager->IsKeyPressed(SDL_SCANCODE_E) - gpInputManager->IsKeyPressed(SDL_SCANCODE_Q)) * 2.0f * dTime;

	if (DEBUG) {
		pC->AddRectColor(pT->mPositionX, pT->mPositionY-24.f, -3.14159 / 2.0f, 48.f, 16.f, 1.f, .4, .8, .4, 0.5f, .2, .8, .2, 0.2f);
	}
}


void Controller::HandleEvent(Event* pEvent) {
	if (pEvent->mType == EventType::COLLIDE) {
		CollideEvent* pCollideEvent = static_cast<CollideEvent*>(pEvent);
		if (pCollideEvent->mChars[1] == mpOwner->GetComponent(TYPE_CHARACTER)) {
			Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
			if (pT != nullptr) {
				// pT->mPositionX = 30.0f;
			}
			
			// PlayerHitEvent phe;
			// gpEventManager->BroadcastEvent(&phe);

			// PlayerHitEvent* pPHE = new PlayerHitEvent();
			// pPHE->mTimer = 2.0f;
			// gpEventManager->AddTimedEvent(pPHE);

			// PlayerHitEvent phe;
			
			/*
			Event* phe = new Event(EventType::PLAYER_HIT);
			gpEventManager->BroadcastEventToSubscribers(phe);
			*/
		}
	}
	
	if (pEvent->mType == EventType::SHOVE) {
		if (pEvent->mChars[1] == mpOwner->GetComponent(TYPE_CHARACTER)) {
			Transform* pT = static_cast<Transform*>(mpOwner->GetComponent(TYPE_TRANSFORM));
			Transform* pTrans = static_cast<Transform*>(pEvent->mChars[0]->mpOwner->GetComponent(TYPE_TRANSFORM));
			
			float ang = atan2f(pTrans->mPositionY - pT->mPositionY, pTrans->mPositionX - pT->mPositionX);
			/*float pushback = 1000.f;
			if (powf(pT->mPositionY - pTrans->mPositionY, 2) + powf(pT->mPositionX - pTrans->mPositionX, 2) <= powf(60.f, 2)) {
				pushback *= 3;
			}*/

			pT->mVelHoriz -= pEvent->mDamage * cosf(ang) *gpFRC->GetDeltaTime();
			pT->mVelVert -= pEvent->mDamage * sinf(ang) *gpFRC->GetDeltaTime();
		}
	}
	
}
