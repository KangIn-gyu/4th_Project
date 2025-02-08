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
	SCENEMANAGER->CreatorScene<TestScene>("TestScene");
	SCENEMANAGER->CreatorScene<GambleScene>("GambleScene");
	SCENEMANAGER->CreatorScene<TitleScene>("TitleScene");
	SCENEMANAGER->CreatorScene<TutorialScene>("TutorialScene");
	SCENEMANAGER->CreatorScene<DialogScene1>("DialogScene1");
	SCENEMANAGER->CreatorScene<LobbyScene>("LobbyScene");
//	TitleScene* titleScene = new TitleScene("Title");
//	SceneManager
	BLACKJACK->player = PLAYER;

	//SCENEMANAGER->ChangeScene("GAMBLE");
	//ENGINE->CollectionGameManager(MYGAMEMANAGER);
	//SOUNDSYSTEM->LoadMusic(eSoundList::Main_Theme, true, "Resource\\STAGE1\\Sounds\\dancer.mp3");
	//SOUNDSYSTEM->SetVolumeAll(0.5f);

	SCENEMANAGER->ChangeScene("DialogScene1");


}
