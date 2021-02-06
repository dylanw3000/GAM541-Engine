#include "AudioManager.h"
#include <stdio.h>
#include <stdlib.h>

AudioManager::AudioManager() 
{
    // Create the Studio System object.
    ERRCHECK(FMOD::Studio::System::create(&system));

    ERRCHECK(system->getCoreSystem(&coreSystem));
    ERRCHECK(coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0));

    // Initialize FMOD Studio, which will also initialize FMOD Core
    ERRCHECK(system->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0));

    FMOD::Studio::Bank* masterBank = NULL;
    ERRCHECK(system->loadBankFile(Common_MediaPath("Master.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &masterBank));

    FMOD::Studio::Bank* stringsBank = NULL;
    ERRCHECK(system->loadBankFile(Common_MediaPath("Master.strings.bank"), FMOD_STUDIO_LOAD_BANK_NORMAL, &stringsBank));

    FMOD::Studio::EventDescription* eventDescription = NULL;
    ERRCHECK(system->getEvent("event:/BGM", &eventDescription));

    FMOD::Studio::EventInstance* eventInstance = NULL;
    ERRCHECK(eventDescription->createInstance(&eventInstance));

    eventInstance->start();
}
AudioManager::~AudioManager()
{}