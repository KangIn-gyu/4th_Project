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
		//BMG
		SOUNDSYSTEM->LoadMusic(eSoundList::TitleScene, true, "Resource\\TitleScene\\Sounds\\Title.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::Credit, true, "Resource\\CreditScene\\Sounds\\Credit.wav");
	}

	{ // Dialog
		//BGM
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

	{ // 버튼 SFX
		SOUNDSYSTEM->LoadMusic(eSoundList::SE_Button_Hover, false, "Resource\\Common\\Sounds\\SE_Button_Hover.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::SE_Button_Click, false, "Resource\\Common\\Sounds\\SE_Button_Click.wav");
	}

	{ // Lobby
		SOUNDSYSTEM->LoadMusic(eSoundList::Lobby, true, "Resource\\LobbyScene\\Sounds\\Lobby.wav");
	}

	{ // InGame Scene
		// BGM
		SOUNDSYSTEM->LoadMusic(eSoundList::GameScene, true, "Resource\\GambleScene\\Sounds\\GameScene.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::DoubleDown, true, "Resource\\GambleScene\\Sounds\\DoubleDown,ShowDown.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::GameOver, true, "Resource\\GambleScene\\Sounds\\GameOver.wav");

		// SFX
		SOUNDSYSTEM->LoadMusic(eSoundList::SE_Card_Pick, false, "Resource\\GambleScene\\Sounds\\SE_Card_Pick.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::SE_Finger, false, "Resource\\GambleScene\\Sounds\\SE_Finger Flip.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::SE_Card_Unfold, false, "Resource\\GambleScene\\Sounds\\SE_Card_Unfold.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::SE_All_In, false, "Resource\\GambleScene\\Sounds\\SE_All In.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::SE_Bet, false, "Resource\\GambleScene\\Sounds\\SE_Bet.wav");

		// VOICE
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_Hit, false, "Resource\\GambleScene\\Sounds\\VS_Hit.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_Double_Down, false, "Resource\\GambleScene\\Sounds\\VS_Double Down.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_Stay, false, "Resource\\GambleScene\\Sounds\\VS_Stay.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_Raise, false, "Resource\\GambleScene\\Sounds\\VS_Raise.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_Fold, false, "Resource\\GambleScene\\Sounds\\VS_Fold.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_Blackjack, false, "Resource\\GambleScene\\Sounds\\VS_Blackjack.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_No_More_Bet, false, "Resource\\GambleScene\\Sounds\\VS_No more bet.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_Win, false, "Resource\\GambleScene\\Sounds\\VS_Win.wav");
		SOUNDSYSTEM->LoadMusic(eSoundList::VS_Lose, false, "Resource\\GambleScene\\Sounds\\VS_Lose.wav");
	}
}
