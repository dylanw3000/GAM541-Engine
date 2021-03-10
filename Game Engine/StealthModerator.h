#pragma once

class StealthModerator {
public:
	StealthModerator();
	~StealthModerator();

public:
	void Update();
	int mStage;
	float mTimer;
	bool mTransition;
	bool mManualOverride;

	int mTransitionTimer, mTransitionTimerLimit;
};
