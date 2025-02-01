#pragma once
#include "SingletonBase.h"

// FMOD
#ifndef _WIN64
#include "inc/fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
using namespace FMOD;
#endif
#ifdef _WIN64
#include "inc/fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
using namespace FMOD;
#endif

#define SOUNDSYSTEM SoundSystem::GetInstance()

enum class eSoundChannel
{
	BGM = 0,
	Effect = 1,
	Voice = 2,
	Size = 3
};

enum class eSoundList
{
	Void = 0,
	Main_Theme = 1, //»ç¿ë
	Size = 2
};

class SoundSystem : public SingletonBase<SoundSystem>
{
	friend class SingletonBase<SoundSystem>;
public:
	void LoadMusic(eSoundList list, bool loopcheck, const char* music);

	void PlayMusic(eSoundList list, eSoundChannel channel);

	void StopMusic(eSoundChannel channel);

	void SetVolumeAll(float volume);

	void SetVolume(float volume, int channel);

	bool isChannelPlaying(eSoundChannel channel);

	void RelaseSounds();

private:
	SoundSystem();

	~SoundSystem();

	FMOD_RESULT result;
	FMOD::System* mSystem;
	FMOD::Channel* mChannel[static_cast<int>(eSoundChannel::Size)];
	FMOD::Sound* mSoundList[static_cast<int>(eSoundList::Size)];
	float mVolume;
};
