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
	SCENEMANAGER->CreatorScene<TestScene>("TestScene"); // 나중에 지워야 됨
	
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
	// TODO : 엔딩 추가 해야됨
	SCENEMANAGER->CreatorScene<LoadingScene>("LoadingScene");

	BLACKJACK->player = PLAYER;

	ENGINE->CollectionGameManager(MYGAMEMANAGER);
	SOUNDSYSTEM->SetVolumeAll(0.5f);

	LoadMusic();
	SCENEMANAGER->ChangeScene("GambleScene");
}

// 사운드 로드하는 짭통
void Client::LoadMusic()
{ 
	{ // 타이틀
		SOUNDSYSTEM->LoadMusic(eSoundList::Main_Theme, true, "Resource\\TitleScene\\Sounds\\Title.wav");
	}
	
}
