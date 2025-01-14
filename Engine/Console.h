#pragma once
#include <Windows.h>

class Console
{
public:
	Console() {}
	~Console() { DestroyConsole(); }

	void CreateConsole(int _width, int _height, int _posx, int _posy);
	void DestroyConsole();
	void ResizeConsole(int _width, int _height);
	void SetFontSize(int _width, int _height);

	void HideConsole();
	void ShowConsole();

	const HWND GetConsoleHwnd() { return consoleHwnd; }
private:
	HWND consoleHwnd {};
};

