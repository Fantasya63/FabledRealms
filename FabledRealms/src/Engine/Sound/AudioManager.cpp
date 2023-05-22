#include "frpch.h"
#include "AudioManager.h"
#include "irrKlang.h"

//Set our instance to null
AudioManager* AudioManager::s_Instance = nullptr;

//Set a variable to hold IrrKlang's sound engine
static irrklang::ISoundEngine* soundEngine = nullptr;

AudioManager::AudioManager()
{
	//Make sure we only have one instance
	FR_CORE_ASSERT(!s_Instance, "AudioManager already Exists!");
	s_Instance = this;

	// start the sound engine with default parameters
	soundEngine = irrklang::createIrrKlangDevice();
}

AudioManager::~AudioManager()
{
	//tell irrKlang to clean up
	soundEngine->drop();
}

void AudioManager::PlaySound(const char* filepath, bool loop)
{
	soundEngine->play2D(filepath, loop);
}

void AudioManager::StopAllSounds()
{
	soundEngine->stopAllSounds();
}
