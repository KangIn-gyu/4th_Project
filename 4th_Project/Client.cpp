#include "pch.h"
#include "Client.h"
#include "../Engine/SceneManager.h"

#include "Player.h"
#include "BlackJack.h"
#include "../Engine/Engine.h"
#include "SceneHeaders.h"
#include "MyGameManager.h"
#include "../Engine/SoundSystem.h"
Client::Client(HINSTANCE _hInstance, std::string_view _GameName, int _screenWidth, int _screenHeight, bool _windoweMode) :
	WindowApp(_hInstance, _GameName, _screenWidth, _screenHeight, _windoweMode)
{

}

Client::~Client()
{
}

void Client::Enter()
{

//	SCENEMANAGER->CreatorScene<TestScene>("TestScene"); // ���߿� ������ ��
	SCENEMANAGER->CreatorScene<LoadingScene>("LoadingScene");
	SCENEMANAGER->ChangeScene("LoadingScene");
	static_cast<LoadingScene*>(SCENEMANAGER->GetScene("LoadingScene"))->NextScene("TitleScene");
	SOUNDSYSTEM->SetVolumeAll(0.5f);
	LoadMusic();
}

void Client::SceneUpload()
{
	SCENEMANAGER->CreatorScene<TitleScene>("TitleScene");
	SCENEMANAGER->CreatorScene<DialogIntro>("DialogIntroScene");
	SCENEMANAGER->CreatorScene<DialogScene0>("DialogScene0");
	SCENEMANAGER->CreatorScene<DialogScene1>("DialogScene1");
	SCENEMANAGER->CreatorScene<TutorialScene>("TutorialScene");
	SCENEMANAGER->CreatorScene<DialogScene2>("DialogScene2");
	SCENEMANAGER->CreatorScene<LobbyScene>("LobbyScene");
	// TODO : talkscene 1,2,3,4,5
	
	SCENEMANAGER->CreatorScene<GambleScene>("GambleScene");

	SCENEMANAGER->CreatorScene<DialogScene3>("DialogScene3");
	SCENEMANAGER->CreatorScene<DialogScene4>("DialogScene4");
	SCENEMANAGER->CreatorScene<DialogScene5>("DialogScene5");

	SCENEMANAGER->CreatorScene<TalkScene1>("TalkScene1");
	SCENEMANAGER->CreatorScene<TalkScene2>("TalkScene2");
	SCENEMANAGER->CreatorScene<TalkScene3>("TalkScene3");
	SCENEMANAGER->CreatorScene<TalkScene4>("TalkScene4");



	// TODO : ���� �߰� �ؾߵ�

	BLACKJACK->player = PLAYER;

	ENGINE->CollectionGameManager(MYGAMEMANAGER);

	SOUNDSYSTEM->SetVolumeAll(0.5f);

	LoadMusic();
}

void Client::LoadMusic()
{ 
	{
		SOUNDSYSTEM->LoadMusic(eSoundList::TitleScene, true, "Resource\\TitleScene\\Sounds\\Title.wav");
	}

	{
		SOUNDSYSTEM->LoadMusic(eSoundList::DialogIntro, true, "Resource\\DialogScenes\\Sounds\\Epilogue_Dialogue_BGM.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::Scene0,		true, "Resource\\DialogScenes\\Sounds\\Scene0_dialogue_BGM.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::Scene1,		true, "Resource\\DialogScenes\\Sounds\\Scene1_Dialogue_BGM.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::Scene2,		true, "Resource\\DialogScenes\\Sounds\\Scene2_Dialogue_BGM.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::Scene3,		true, "Resource\\DialogScenes\\Sounds\\Scene3_Dialogue_BGM.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::Scene4,		true, "Resource\\DialogScenes\\Sounds\\Scene4_Dialogue_BGM.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::Scene5,		true, "Resource\\DialogScenes\\Sounds\\Scene5_Dialogue_BGM.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::Ending,		true, "Resource\\DialogScenes\\Sounds\\Ending_Dialogue_BGM.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::BadEnding,	true, "Resource\\DialogScenes\\Sounds\\BadEnding_Dialogue_BGM.mp3");
	}
}
