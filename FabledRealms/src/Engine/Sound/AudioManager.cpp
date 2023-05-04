#include "frpch.h"
#include "AudioManager.h"
#include "irrKlang.h"

AudioManager* AudioManager::s_Instance = nullptr;

static irrklang::ISoundEngine* soundEngine = nullptr;

AudioManager::AudioManager()
{
	FR_CORE_ASSERT(!s_Instance, "AudioManager already Exists!");
	s_Instance = this;

	// start the sound engine with default parameters
	soundEngine = irrklang::createIrrKlangDevice();
}

AudioManager::~AudioManager()
{
	//Cast the typeless void* to Irrklang's sound engine then tell irrKlang to clean up
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
