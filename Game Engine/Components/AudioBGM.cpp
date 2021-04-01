#include "AudioBGM.h"

extern AudioManager* gpAudioManager;

/// <summary>
/// AudioClip
/// </summary>
AudioBGM::AudioBGM() : Component(TYPE_AUDIOCLIP)
{
	
}

AudioBGM::~AudioBGM()
{}



void AudioBGM::Serialize(rapidjson::GenericArray<false, rapidjson::Value>input)
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

void AudioBGM::Update() {}

void AudioBGM::PlayOneShot(std::string eventName)
{
	for (auto audioEvent : mEventList)
	{
		if (audioEvent->mEventName == eventName)
		{
			audioEvent->mEventInstance->start();
		}
	}
}

