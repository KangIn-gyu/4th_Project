#pragma once
#include <windows.h>

#include <string>
#include <string_view>
#include <memory>

#include "Console.h"


//목적 : 기반 클래스 이걸 상속해서 윈도우 생성
struct WindowInfo;
class WindowApp
{
// 함수
public:
	WindowApp(HINSTANCE _hInstance, std::string_view _gameName, int _screenWidth, int _screenHeight, bool _windoweMode);
	~WindowApp();

	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	HINSTANCE& GethInstance() { return hInstance; }
	std::string GetWindowClassName() const { return windowClassName; }
	WindowInfo* GetWindowInfo();

	virtual void Enter() {}; // 씬 로드 하는 곳
	void SetWindowSize(int _width, int _height);
private:
	void Initialize();
	ATOM WindowsRegistration();
	
//변수 
public:
	static Console* console;

protected:

private:
	HINSTANCE hInstance = nullptr;
	WindowInfo*	windowInfo {};

	std::string windowClassName = "4th_Project";
	std::string gameName;
	
};
