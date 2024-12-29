#include "pch.h"
#include "WindowApp.h"
#include "Console.h"
#include "Engine.h"
#include "Helper.h"
#include "Declare.h"
// 다이렉트
#include <directxtk/Mouse.h>
#include <directxtk/Keyboard.h>

// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 용도 : WindowManager를 파생 클래스가 생성이 되면 풀스크린이 아니고 디버그 모드면 콘솔창을 생성한다.
static Console* g_Console {}; 

WindowApp::WindowApp(HINSTANCE _hInstance, std::string_view _GameName, int _screenWidth, int _screenHeight, bool _windoweMode) : \
    hInstance(_hInstance), GamName(_GameName)
{
    windowInfo = new WindowInfo;
    windowInfo->screenWidth = _screenWidth;
    windowInfo->screenHeight = _screenHeight;
    windowInfo->windoweMode = _windoweMode;
    ENGINE->SetWindowApp(this);
    WindowsRegistration();
    Initialize(); 

#if(_DEBUG) // 창모드일 경우 안나오게
    if(true == _windoweMode)
    {
        console = std::make_unique<Console>();
        g_Console = console.get();
        RECT mainWindowRect {};

        if (nullptr != windowInfo->hWnd)
        {
            GetWindowRect(windowInfo->hWnd, &mainWindowRect);
        }
  
        int consoleX = mainWindowRect.right;                            // 메인 창의 오른쪽 끝
        int consoleY = mainWindowRect.top;                              // 메인 창의 Y 위치
        int consoleWidth = 400;                                         // 콘솔 창 너비
        int consoleHeight = mainWindowRect.bottom - mainWindowRect.top; // 메인 창과 동일한 높이
        console->CreateConsole(consoleX, consoleY, consoleWidth, consoleHeight);
    } // 추후 계획 ImGui에 넣어서 버튼 클릭하면 나오게 처리할 예정
#endif
}

WindowApp::~WindowApp()
{
    SafeExtinction::SAFE_DELETE(windowInfo);
}

LRESULT WindowApp::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
//    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
//        return true;
    switch (_message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_EXITSIZEMOVE:
    {
        if (nullptr != g_Console)
        {
            // 메인 윈도우의 현재 위치를 가져옵니다.
            RECT mainRect;
            GetWindowRect(_hWnd, &mainRect);

            // 콘솔 창을 메인 윈도우 오른쪽으로 이동시킵니다.
            int consoleX = mainRect.right;    // 메인 윈도우 오른쪽 끝
            int consoleY = mainRect.top;      // 메인 윈도우의 Y 위치
            SetWindowPos(g_Console->GetConsoleHwnd(), nullptr, consoleX, consoleY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
        }
    }
        break;
    case WM_SETFOCUS:
        break;
    case WM_KILLFOCUS:
        break;
    case WM_ENTERSIZEMOVE:
        break;
    case WM_ACTIVATEAPP:
        DirectX::Keyboard::ProcessMessage(_message, _wParam, _lParam);
        DirectX::Mouse::ProcessMessage(_message, _wParam, _lParam);
        break;

    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
        DirectX::Mouse::ProcessMessage(_message, _wParam, _lParam);
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
        DirectX::Keyboard::ProcessMessage(_message, _wParam, _lParam);
        break;

    default:
        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
    }
    return 0;
}

WindowInfo* WindowApp::GetWindowInfo() const
{
    return windowInfo;
}

bool WindowApp::Initialize()
{
    RECT rcClient = { 0,0, windowInfo->screenWidth , windowInfo->screenHeight };
    AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

    int midX = (GetSystemMetrics(SM_CXSCREEN) - windowInfo->screenWidth) / 2;
    int midY = (GetSystemMetrics(SM_CYSCREEN) - windowInfo->screenHeight) / 2;

    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;
      
    if(true == windowInfo->windoweMode)
    {
        windowInfo->hWnd = CreateWindowEx(0, StringConverter::StringToWide(windowClassName).c_str(),
            StringConverter::StringToWide(GamName).c_str(), dwStyle,
            midX, midY, rcClient.right - rcClient.left,
            rcClient.bottom - rcClient.top, NULL, NULL, hInstance, NULL);
    }
    else
    {
        windowInfo->screenWidth = GetSystemMetrics(SM_CXSCREEN);
        windowInfo->screenHeight = GetSystemMetrics(SM_CYSCREEN);

        windowInfo->hWnd = CreateWindowEx(0, StringConverter::StringToWide(windowClassName).c_str(),
            StringConverter::StringToWide(GamName).c_str(),
            WS_EX_TOPMOST | WS_POPUP, 0, 0,
            GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
            NULL, NULL, hInstance, NULL);
    }

    if (!windowInfo->hWnd) { return FALSE; }
   
    SetWindowLongPtr(windowInfo->hWnd, GWL_STYLE, dwStyle);   // 창 크기 조정 비활성화: 창 스타일 변경
    ShowWindow(windowInfo->hWnd, SW_SHOW);
    UpdateWindow(windowInfo->hWnd);

    // 윈도우를 화면에 표시하고 포커스를 지정
    SetFocus(windowInfo->hWnd);
    SetForegroundWindow(windowInfo->hWnd);
    return TRUE;
}

ATOM WindowApp::WindowsRegistration()
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.hInstance = hInstance;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = this->WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"4th_Project";

    return RegisterClassExW(&wcex);
}
