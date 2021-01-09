/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: FrameRateController.cpp
Purpose: Maintains the frame rate and tracks delta_time
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_1
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 5, 2020
- End Header --------------------------------------------------------*/

#include "FrameRateController.h"
#include "limits.h"
#include <SDL_timer.h>

FrameRateController::FrameRateController(unsigned int MaxFrameRate) {
	if (MaxFrameRate == 0)
		MaxFrameRate = UINT_MAX;

	mMaxFrameRate = MaxFrameRate;
	mTicksPerFrame = 1000 / mMaxFrameRate;
	mTickStart = 0;
	mTickEnd = 0;
	mFrameTime = 0;
	mDeltaTime = 0.0f;
}

FrameRateController::~FrameRateController() {
	//
}

void FrameRateController::FrameStart() {
	mTickStart = SDL_GetTicks();
}

void FrameRateController::FrameEnd() {
	mTickEnd = SDL_GetTicks();
	while (mTickEnd - mTickStart < mTicksPerFrame)
		mTickEnd = SDL_GetTicks();

	mFrameTime = mTickEnd - mTickStart;
	mDeltaTime = (float) mFrameTime / 1000;
}

unsigned int FrameRateController::GetFrameTime() {
	return mFrameTime;
}

float FrameRateController::GetDeltaTime() {
	return mDeltaTime;
}
