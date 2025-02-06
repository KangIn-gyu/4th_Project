#include "pch.h"
#include "Client.h"
#include "../Engine/Engine.h"

#include "TestScene.h"
#include "GambleScene.h"
#include "Player.h"
#include "BlackJack.h"

#include "IntroCutScene.h"

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
	IntroCutScene* intorCutScene = new IntroCutScene("IntroCut");

	BLACKJACK->player = PLAYER;
	ENGINE->ChangeScene("IntroCut");
}
