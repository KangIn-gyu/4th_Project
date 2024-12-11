#pragma once
#include <windows.h>

#include <string>
#include <string_view>
#include <memory>

#include "Console.h"

//목적 : 기반 클래스 이걸 상속해서 윈도우 생성
class WindowApp
{
// 함수
public:
	WindowApp(HINSTANCE hInstance, std::string_view GameName, int ScreenWidth, int ScreenHeight, bool isFullScreen);
	~WindowApp();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetWindowHandle() { return hWnd; }
	HINSTANCE& GethInstance() { return hInstance; }
	std::string GetWindowClassName() { return windowClassName; }
protected:
	bool Initialize();

private:
	ATOM WindowsRegistration();
	
//변수 
public:

protected:
	std::unique_ptr<Console> console {};

private:
	int	screenWidth {};
	int	screenHeight {};

	HWND hWnd = nullptr;  // 메인 핸들
	HINSTANCE hInstance = nullptr;

	std::string windowClassName = "4th_Project";
	std::string GamName;
	bool isFullScreen = false;
};
