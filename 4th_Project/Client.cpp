#include "pch.h"
#include "Client.h"

Client::Client(HINSTANCE hInstance, std::string_view GameName, int ScreenWidth, int ScreenHeight, bool isFullScreen) :
	WindowApp(hInstance, GameName, ScreenWidth, ScreenHeight, isFullScreen)
{

}

Client::~Client()
{
}
