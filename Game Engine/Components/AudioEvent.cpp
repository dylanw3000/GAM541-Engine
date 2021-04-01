#include "AudioClip.h"

extern AudioManager* gpAudioManager;

AudioEvent::AudioEvent()
{}

AudioEvent::~AudioEvent()
{}

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