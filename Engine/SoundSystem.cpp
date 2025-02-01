#include "pch.h"
#include "SoundSystem.h"

void SoundSystem::LoadMusic(eSoundList soundlist, bool loopcheck, const char* music)
{
	if (loopcheck)
		mSystem->createSound(music, FMOD_LOOP_NORMAL, 0, &mSoundList[(int)(soundlist)]);
	else
		mSystem->createSound(music, FMOD_LOOP_OFF, 0, &mSoundList[(int)(soundlist)]);
}

void SoundSystem::PlayMusic(eSoundList soundlist, eSoundChannel channel)
{
	int k = (int)(channel);
	mChannel[k]->stop();
	mSystem->playSound(mSoundList[(int)(soundlist)], nullptr, false, &mChannel[k]);
	mChannel[k]->setVolume(mVolume);
}

void SoundSystem::StopMusic(eSoundChannel channel)
{
	mChannel[(int)(channel)]->stop();
}

void SoundSystem::SetVolumeAll(float volume)
{
	mVolume = volume;
	for (unsigned int i = 0; i < (int)(eSoundChannel::Size); ++i)
		mChannel[i]->setVolume(mVolume);
	//mChannel[(int)channel]->setVolume(mVolume);
}

void SoundSystem::SetVolume(float volume, int channel)
{
	mVolume = volume;
	if (mVolume <= 0)
	{
		mVolume = 0;
	}
	if (mVolume >= 1)
	{
		mVolume = 1;
	}
	/*for (unsigned int i = 0; i < (int)(eSoundChannel::Size); ++i)
		mChannel[i]->setVolume(mVolume);*/
	mChannel[channel]->setVolume(mVolume);
}



void SoundSystem::RelaseSounds()
{
	for (int i = 0; i < (int)eSoundChannel::Size; i++) {
		mChannel[i]->stop();
	}
	//https://www.fmod.com/docs/2.03/api/core-api-sound.html#sound_release 참고.
	//소리가 재생중이면 해제를 보장하지 못 하므로 stop() 시행 후 해제. 
	for (int i = 0; i < (int)eSoundList::Size; i++) {
		if (mSoundList[i] != nullptr) {

			mSoundList[i]->release();
		}
	}
}

bool SoundSystem::isChannelPlaying(eSoundChannel channel) {
	bool isPlaying = false;
	mChannel[(int)channel]->isPlaying(&isPlaying);
	return isPlaying;
}

SoundSystem::SoundSystem() : mSystem(), mChannel{}, mSoundList{}, mVolume()
{
	System_Create(&mSystem);
	mSystem->init(6, FMOD_INIT_NORMAL, 0);
}

SoundSystem::~SoundSystem()
{
	//mSystem->close();
	mSystem->release();
	//https://www.fmod.com/docs/2.03/api/core-api-system.html 참고.
	//release가 close를 겸함.

}
