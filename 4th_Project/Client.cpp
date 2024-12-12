#include "pch.h"
#include "Client.h"

Client::Client(HINSTANCE hInstance, std::wstring_view GameName, int ScreenWidth, int ScreenHeight, bool isFullScreen) :
	WindowManager(hInstance, GameName, ScreenWidth, ScreenHeight, isFullScreen)
{

}

Client::~Client()
{
}
