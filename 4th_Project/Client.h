#pragma once
#include "../Engine/WindowApp.h"
#include <string_view>

class Client : public WindowApp
{
	// 함수
public:
	Client(HINSTANCE hInstance, std::string_view GameName, int ScreenWidth, int ScreenHeight, bool isFullScreen);
	~Client();

private:

// 변수
public:

private:
};

