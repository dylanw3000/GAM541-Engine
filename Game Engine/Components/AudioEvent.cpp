#include "AudioClip.h"

extern AudioManager* gpAudioManager;

AudioEvent::AudioEvent()
{}

AudioEvent::~AudioEvent()
{}

AudioEvent::AudioEvent(std::string eventName, float eventVolume)
{
	mEventName = eventName;
	mEventVolume = eventVolume;
	ERRCHECK(gpAudioManager->system->getEvent(("event:/" + mEventName).c_str(), &mEventDescription));
	ERRCHECK(mEventDescription->createInstance(&mEventInstance));
	ERRCHECK(mEventInstance->setVolume(mEventVolume));
}


void AudioEvent::SetData(rapidjson::Value& input)
{
	if (input.HasMember("name"))
	{
		mEventName = input["name"].GetString();
		ERRCHECK(gpAudioManager->system->getEvent(("event:/" + mEventName).c_str(), &mEventDescription));
	}
	if (input.HasMember("volume"))
	{
		mEventVolume = input["volume"].GetFloat();
	}
	ERRCHECK(mEventDescription->createInstance(&mEventInstance));
	ERRCHECK(mEventInstance->setVolume(mEventVolume));
}