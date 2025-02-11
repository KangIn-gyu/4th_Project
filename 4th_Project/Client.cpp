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

//	SCENEMANAGER->CreatorScene<TestScene>("TestScene"); // ³ªÁß¿¡ Áö¿ö¾ß µÊ
	SCENEMANAGER->CreatorScene<LoadingScene>("LoadingScene");
	SCENEMANAGER->ChangeScene("LoadingScene");

	SOUNDSYSTEM->SetVolumeAll(0.5f);
	LoadMusic();
}

void Client::SceneUpload()
{
//	SCENEMANAGER->CreatorScene<TestScene>("TestScene"); // ?˜ì¤‘??ì§€?Œì•¼ ??
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

	// TODO : ¿£µù Ãß°¡ ÇØ¾ßµÊ

	SCENEMANAGER->CreatorScene<LoadingScene>("LoadingScene");

	BLACKJACK->player = PLAYER;

	ENGINE->CollectionGameManager(MYGAMEMANAGER);

	SOUNDSYSTEM->SetVolumeAll(0.5f);

	LoadMusic();

	SCENEMANAGER->ChangeScene("DialogIntroScene");
}

void Client::LoadMusic()
{ 
	{
		SOUNDSYSTEM->LoadMusic(eSoundList::Main_Theme, true, "Resource\\TitleScene\\Sounds\\Title.wav");
	}
}
