#pragma once
#include "../Engine/WindowManager.h"
#include <string_view>

class Client : public WindowManager
{
	// 함수
public:
	Client(HINSTANCE hInstance, std::wstring_view GameName, int ScreenWidth, int ScreenHeight, bool isFullScreen);
	~Client();

private:

// 변수
public:

private:
};

