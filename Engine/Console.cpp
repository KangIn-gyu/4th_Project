#include "pch.h"
#include "Console.h"

void Console::CreateConsole(int posx, int posy, int width, int height)
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONIN$", "r", stdin);
	freopen_s(&fp, "CONOUT$", "w", stderr);

	hConsoleWindow = GetConsoleWindow(); // 콘솔 창 핸들 가져오기
	SetWindowPos(hConsoleWindow, nullptr , posx, posy, width, height, SWP_NOZORDER);

	LONG style = GetWindowLong(hConsoleWindow, GWL_STYLE);
	style &= ~(WS_CAPTION | WS_THICKFRAME);  // 타이틀 바와 사이즈 조정 프레임 제거
	SetWindowLong(hConsoleWindow, GWL_STYLE, style);
}

void Console::DestroyConsole()
{
	FreeConsole();
}

void Console::SetFontSize(int width, int height)
{
	// 콘솔 핸들 가져오기
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// 콘솔 글꼴 크기 설정
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(CONSOLE_FONT_INFOEX) };
	fontInfo.dwFontSize.X = width;  // 글꼴 너비
	fontInfo.dwFontSize.Y = 15; // 글꼴 높이
	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_BOLD;
	wcscpy_s(fontInfo.FaceName, L"Logger");
	SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}
