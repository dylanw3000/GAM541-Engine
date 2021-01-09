#pragma once

class Moderator {
public:
	Moderator();
	~Moderator();

public:
	void Update();
	int mStage;
	float mTimer;
	bool mTransition;
};
