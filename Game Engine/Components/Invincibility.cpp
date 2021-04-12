#include "Invincibility.h"
#include "..\FrameRateController.h"


extern FrameRateController* gpFRC;

Invincibility::Invincibility() : Component(TYPE_INVINCIBILITY)
{
	
}

Invincibility::~Invincibility()
{}



void Invincibility::Serialize(rapidjson::GenericArray<false, rapidjson::Value>input)
{
}

void Invincibility::SetInvincible(float a)
{
	invinceTimer = a;
}

void Invincibility::Update()
{
	invinceTimer -= gpFRC->GetDeltaTime();
	if (invinceTimer > 0.0f) 
	{
		isInvincible = true;
	}
	else
	{
		isInvincible = false;
	}
}

