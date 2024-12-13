#pragma once
#include "../Engine/WindowApp.h"
#include <string_view>

class Client : public WindowApp
{
	// 함수
public:
	Client(HINSTANCE _hInstance, std::string_view _GameName, int _screenWidth, int _screenHeight, bool _windoweMode);
	~Client();

private:

// 변수
public:

private:
};

