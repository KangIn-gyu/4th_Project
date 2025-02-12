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
	Effect2 = 3,
	Effect3 = 4,
	Size = 3
};

enum class eSoundList
{
	Void = 0,
	TitleScene = 1, //사용
	DialogIntro = 2, //사용
	Scene0 = 3,
	Scene1 = 4,
	Scene2 = 5,
	Scene3,
	Scene4,
	Scene5,
	Ending,
	BadEnding,
	SE_Card_Flap,
	SE_Button_Hover,
	SE_Button_Click,
	SE_Finger,
	SE_Card_Gather,
	Lobby,
	GameScene,
	DoubleDown,
	GameOver,
	SE_Card_Pick,
	SE_Card_Unfold,
	VS_Hit,
	VS_Double_Down,
	VS_Stay,
	VS_Raise,
	VS_Fold,
	VS_Blackjack,
	VS_No_More_Bet,
	VS_Win,
	VS_Lose,
	SE_All_In,
	SE_Bet,
	Credit,
	Size
};

class SoundSystem : public SingletonBase<SoundSystem>
{
	friend class SingletonBase<SoundSystem>;
public:
	void Init();

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
