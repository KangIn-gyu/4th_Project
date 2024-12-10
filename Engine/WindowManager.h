#pragma once
#include <windows.h>
#include <string>
#include <string_view>

//목적 : 기반 클래스 이걸 상속해서 윈도우 생성
class WindowManager
{
// 함수
public:
	WindowManager(HINSTANCE hInstance, std::wstring_view GameName, int ScreenWidth, int ScreenHeight, bool isFullScreen);
	~WindowManager();

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	HWND GetWindowHandle() { return m_hWnd; }
	
protected:
	bool Initialize();

private:
	ATOM WindowsRegistration();
	

//변수 
public:

private:
	int	m_fScreenWidth {};
	int	m_fScreenHeight {};

	HWND m_hWnd = nullptr;  // 메인 핸들
	HINSTANCE m_hInstance = nullptr;

	std::wstring m_strGamName;
	bool m_isFullScreen = false;
};

