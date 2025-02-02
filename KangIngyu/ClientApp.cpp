#include "pch.h"
#include "ClientApp.h"
#include "KangScene.h"
#include "../Engine/Engine.h"
#include "../Engine/SoundSystem.h"

ClientApp::ClientApp(HINSTANCE _hInstance, std::string_view _GameName, int _screenWidth, int _screenHeight, bool _windoweMode) :
	WindowApp(_hInstance, _GameName, _screenWidth, _screenHeight, _windoweMode)
{
}

ClientApp::~ClientApp()
{

}

void ClientApp::Enter()
{
	//SOUNDSYSTEM->LoadMusic(eSoundList::Main_Theme, true, "Resource/Sound/littleroot.wav");
	//SOUNDSYSTEM->SetVolumeAll(0.01f);
	KangScene* testScene = new KangScene("TEST");
	ENGINE->ChangeScene("TEST");
}
