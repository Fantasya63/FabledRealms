#pragma once

class AudioManager
{
public:
	AudioManager();
	~AudioManager();

	static AudioManager& Get() { return *s_Instance; }

	void PlaySound(const char* filepath, bool loop);
	void StopAllSounds();
	
private:
	static AudioManager* s_Instance;
};

