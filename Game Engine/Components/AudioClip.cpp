#include "AudioClip.h"

extern AudioManager* gpAudioManager;

AudioClip::AudioClip() : Component(TYPE_AUDIOCLIP)
{
	
}

AudioClip::~AudioClip()
{}

void AudioClip::Serialize(rapidjson::GenericArray<false, rapidjson::Value>input)
{
	if (input[0].HasMember("name"))
	{
		std::string mEventName = input[0]["name"].GetString();
		ERRCHECK(gpAudioManager->system->getEvent(("event:/" + mEventName).c_str(), &mEventDescription));
	}
	if (input[0].HasMember("volume"))
	{
		mVolume = input[0]["volume"].GetFloat();
	}

	ERRCHECK(mEventDescription->createInstance(&mEventInstance));
	ERRCHECK(mEventInstance->setVolume(mVolume));
}

void AudioClip::Update() {}

void AudioClip::PlayOneShot() 
{
	mEventInstance->start();
}

