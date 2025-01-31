#pragma once
#include "../Engine/WindowApp.h"

class ClientApp : public WindowApp
{
public:
	ClientApp(HINSTANCE _hInstance, std::string_view _GameName, int _screenWidth, int _screenHeight, bool _windoweMode);
	~ClientApp();
	virtual void Enter() override;


};

