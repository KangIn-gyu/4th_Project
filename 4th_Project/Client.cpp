#include "pch.h"
#include "Client.h"
#include "TestScene.h"
#include "../Engine/Engine.h"
#include "GambleScene.h"
#include "Player.h"
#include "BlackJack.h"

Client::Client(HINSTANCE _hInstance, std::string_view _GameName, int _screenWidth, int _screenHeight, bool _windoweMode) :
	WindowApp(_hInstance, _GameName, _screenWidth, _screenHeight, _windoweMode)
{

}

Client::~Client()
{
}

void Client::Enter()
{
	TestScene* testScene = new TestScene("TEST");
	GambleScene* gamblescne = new GambleScene("GAMBLE");
	BLACKJACK->player = PLAYER;
	ENGINE->ChangeScene("TEST");
}
