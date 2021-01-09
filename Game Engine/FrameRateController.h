/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: FrameRateController.h
Purpose: Maintains the frame rate and tracks delta_time
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_1
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 5, 2020
- End Header --------------------------------------------------------*/

#pragma once


class FrameRateController {
public:
	FrameRateController(unsigned int MaxFrameRate);
	~FrameRateController();

	void FrameStart();
	void FrameEnd();
	unsigned int GetFrameTime();
	float GetDeltaTime();
public:


private:
	unsigned int mTickStart;
	unsigned int mTickEnd;
	unsigned int mMaxFrameRate;
	unsigned int mTicksPerFrame;
	unsigned int mFrameTime;
	float mDeltaTime;
private:

};
