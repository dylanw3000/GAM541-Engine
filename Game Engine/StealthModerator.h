#pragma once

class StealthModerator {
public:
	StealthModerator();
	~StealthModerator();

public:
	void Update();
	int mStage,mLastStage;
	float mTimer;
	bool mTransition;
	bool mManualOverride, mManualBack, mManualRestart;

	int mTransitionTimer, mTransitionTimerLimit;
};
