/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Slime.h
Purpose: Handles movement patterns for a "slime" object
Language: C++, gpp
Platform: gpp
Project: dylan.washburne CS529_milestone_2
Author: Dylan Washburne, dylan.washburne, 60001820
Creation date: October 15, 2020
- End Header --------------------------------------------------------*/

#pragma once

#include "Component.h"

class Objective : public Component {
public:
	Objective();
	~Objective();

	void Update();
	void Serialize(std::ifstream& InputStream);
	void Serialize(rapidjson::GenericArray<false, rapidjson::Value>);

	void HandleEvent(Event* pEvent);

public:
	bool mCompleted;


private:
	float mRadius;
private:
};
