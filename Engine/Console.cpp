#include "pch.h"
#include "Console.h"

void Console::CreateConsole(int _posx, int _posy, int _width, int _height)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stderr);

	consoleHwnd = GetConsoleWindow(); // 콘솔 창 핸들 가져오기
	SetWindowPos(consoleHwnd, nullptr , _posx, _posy, _width, _height, SWP_NOZORDER);

	LONG style = GetWindowLong(consoleHwnd, GWL_STYLE);
	style &= ~(WS_CAPTION | WS_THICKFRAME);  // 타이틀 바와 사이즈 조정 프레임 제거
	SetWindowLong(consoleHwnd, GWL_STYLE, style);
}

void Console::DestroyConsole()
{
	FreeConsole();
}

void Console::ResizeConsole(int _width, int _height)
{
	// 콘솔 핸들 가져오기
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD newSize;
	newSize.X = _width;
	newSize.Y = _height * 2;
	SetConsoleScreenBufferSize(hConsole, newSize);

	// 콘솔 창 크기 조정
	SMALL_RECT windowSize = { 0, 0, _width - 1, _height - 1 };
	SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

	// 콘솔 창 위치 및 크기 설정
	//  _width * 8, _height * 16
	SetWindowPos(consoleHwnd, nullptr, 0, 0, _width, _height , SWP_NOMOVE | SWP_NOZORDER);
}

void Console::SetFontSize(int _width, int _height)
{
	// 콘솔 핸들 가져오기
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 콘솔 글꼴 크기 설정
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	fontInfo.dwFontSize.X = _width;  // 글꼴 너비
	fontInfo.dwFontSize.Y = _height; // 글꼴 높이
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_BOLD;
	wcscpy_s(fontInfo.FaceName, L"Logger");
	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void Console::HideConsole()
{
	ShowWindow(consoleHwnd, SW_HIDE); // 콘솔 창 숨기기
}

void Console::ShowConsole()
{
	ShowWindow(consoleHwnd, SW_SHOW); // 콘솔 창 표시하기
}
