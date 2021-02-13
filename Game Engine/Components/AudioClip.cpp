#include "AudioClip.h"

AudioClip::AudioClip() : Component(TYPE_BODY)
{
	
}

AudioClip::~AudioClip()
{}

void AudioClip::Serialize(rapidjson::GenericArray<false, rapidjson::Value>) {}

void AudioClip::Update() {}