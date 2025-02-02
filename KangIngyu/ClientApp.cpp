#include "pch.h"
#include "ClientApp.h"
#include "KangScene.h"
#include "../Engine/Engine.h"

ClientApp::ClientApp(HINSTANCE _hInstance, std::string_view _GameName, int _screenWidth, int _screenHeight, bool _windoweMode) :
	WindowApp(_hInstance, _GameName, _screenWidth, _screenHeight, _windoweMode)
{
}

ClientApp::~ClientApp()
{

}

void ClientApp::Enter()
{
	KangScene* testScene = new KangScene("TEST");
	ENGINE->ChangeScene("TEST");
}
