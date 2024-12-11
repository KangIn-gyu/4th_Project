#pragma once
#include <Windows.h>

class Console
{
public:
	Console() {}
	~Console() { DestroyConsole(); }

	void CreateConsole(int width, int height, int posx, int posy);
	void DestroyConsole();
	void SetFontSize(int width, int height);

	const HWND GetConsoleHwnd() { return consoleHwnd; }
private:
	HWND consoleHwnd {};
};

