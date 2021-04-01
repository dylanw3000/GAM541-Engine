#include "AudioClip.h"

extern AudioManager* gpAudioManager;


AudioClip::AudioClip() : Component(TYPE_AUDIOCLIP)
{
	
}

AudioClip::~AudioClip()
{}



void AudioClip::Serialize(rapidjson::GenericArray<false, rapidjson::Value>input)
{
	if (input[0].HasMember("EventCount"))
	{
		mEventCount = input[0]["EventCount"].GetInt();
	}

	for (int i = 1; i <= mEventCount; i++)
	{
		AudioEvent* newEvent = new AudioEvent();
		newEvent->SetData(input[i]);
		mEventList.push_back(newEvent);
	}
}

void AudioClip::Update() {}

void AudioClip::PlayOneShot(std::string eventName) 
{
	for (auto audioEvent : mEventList)
	{
		if (audioEvent->mEventName == eventName)
		{
			audioEvent->mEventInstance->start();
		}
	}
}

