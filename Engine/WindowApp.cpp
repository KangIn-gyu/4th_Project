#include "pch.h"
#include "WindowApp.h"
#include "Console.h"
#include "Engine.h"
#include "Helper.h"
#include "Declare.h"
#include "Engine.h"
#include "UserImGui.h"
// 다이렉트
#include <directxtk/Mouse.h>
#include <directxtk/Keyboard.h>
#include "DirectXInput.h"
#include "EventSystem.h"
// 용도 : WindowManager를 파생 클래스가 생성이 되면 풀스크린이 아니고 디버그 모드면 콘솔창을 생성한다.
Console* WindowApp::console = nullptr;
WindowApp::WindowApp(HINSTANCE _hInstance, std::string_view _gameName, int _screenWidth, int _screenHeight, bool _windoweMode) : \
    hInstance(_hInstance), gameName(_gameName)
{
    windowInfo = new WindowInfo;
    windowInfo->screenWidth = _screenWidth;
    windowInfo->screenHeight = _screenHeight;
    windowInfo->windoweMode = _windoweMode;
    ENGINE->SetWindowApp(this);

    WindowsRegistration();
    Initialize(); 

// 강인규가 잠시 수정해놓음 25.2.8 
#if(_DEBUG) // 창모드일 경우 안나오게
   if(true == _windoweMode)
   {
       //console = new Console;
       //RECT mainWindowRect {};
   
       //if (nullptr != windowInfo->hWnd)
       //{
       //    GetWindowRect(windowInfo->hWnd, &mainWindowRect);
       //}
       //int consoleX = mainWindowRect.right;                            // 메인 창의 오른쪽 끝
       //int consoleY = mainWindowRect.top;                              // 메인 창의 Y 위치
       //int consoleWidth = 400;                                         // 콘솔 창 너비
       //int consoleHeight = mainWindowRect.bottom - mainWindowRect.top; // 메인 창과 동일한 높이
       //console->CreateConsole(consoleX, consoleY, consoleWidth, consoleHeight);
   } // 추후 계획 ImGui에 넣어서 버튼 클릭하면 나오게 처리할 예정
#endif
}

WindowApp::~WindowApp()
{
    SafeExtinction::SAFE_DELETE(console);
    SafeExtinction::SAFE_DELETE(windowInfo);
}

bool isDragging = false;
bool isClick = false;
int dragThresholdX;
int dragThresholdY;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowApp::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
    ImGui_ImplWin32_WndProcHandler(_hWnd, _message, _wParam, _lParam);
   
    POINT startPoint{ eventSysyem->startPoint };
    /*isDragging = eventSysyem->isDragging;
    isClick = eventSysyem->isClick;*/
    dragThresholdX = GetSystemMetrics(SM_CXDRAG) * 5;
    dragThresholdY = GetSystemMetrics(SM_CYDRAG) * 5; //드래그 임계값 5를바꾸면 드래그 감도?조절
    switch (_message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SIZE:
    {
        UINT newWidth = LOWORD(_lParam);
        UINT newHeight = HIWORD(_lParam);

        if (nullptr != WindowApp::console)
        { // 여기서 윈도우 사이즈 변경에 대해서 다 처리함
            IMGUI->SetWindowSize(newWidth, newHeight);
        }
        if (nullptr != ENGINE->clientApp)
        {
            ENGINE->SetWindowSize(newWidth, newHeight);
        }
    }
    break;
    case WM_EXITSIZEMOVE:
    {
        if (nullptr != WindowApp::console)
        {
            // 메인 윈도우의 현재 위치를 가져옵니다.
            RECT mainRect;
            GetWindowRect(_hWnd, &mainRect);

            // 콘솔 창을 메인 윈도우 오른쪽으로 이동시킵니다.
            int consoleX = mainRect.right;    // 메인 윈도우 오른쪽 끝
            int consoleY = mainRect.top;      // 메인 윈도우의 Y 위치
            SetWindowPos(WindowApp::console->GetConsoleHwnd(), nullptr, consoleX, consoleY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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
        DirectX::Mouse::ProcessMessage(_message, _wParam, _lParam);
        if (_wParam & MK_LBUTTON) // 마우스 왼쪽 버튼이 눌린 상태
        {
            int x = DXINPUT.get()->mouseState.x;
            int y = DXINPUT.get()->mouseState.y;
            if (abs(x - eventSysyem->startPoint.x) > dragThresholdX || abs(y - eventSysyem->startPoint.y) > dragThresholdY) //드래그 임계값
            {
                isDragging = true;
                isClick = false;
            }
            if (eventSysyem->isDragging != isDragging)
                eventSysyem->BeginDrag();
            if (isDragging)
                eventSysyem->StayDrag();
            eventSysyem->isDragging = isDragging;
        }
        break;
    case WM_LBUTTONDOWN:

        DirectX::Mouse::ProcessMessage(_message, _wParam, _lParam);
        eventSysyem->startPoint.x = DXINPUT.get()->mouseState.x;
        eventSysyem->startPoint.y = DXINPUT.get()->mouseState.y;
        isClick = true;
        isDragging = false;
        break;
    case WM_LBUTTONUP:
        DirectX::Mouse::ProcessMessage(_message, _wParam, _lParam);
        if (isClick)
        {
            eventSysyem->checkClickobj(DXINPUT.get()->mouseState.x, DXINPUT.get()->mouseState.y);
        }
        else if(isDragging)
        {
            // 드래그 종료 처리
            eventSysyem->isDragging = false;
            eventSysyem->EndDrag();
        }
        break;
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
        DirectX::Mouse::ProcessMessage(_message, _wParam, _lParam);
        break;
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

    case WM_DPICHANGED:
        break;
    default:
        return DefWindowProc(_hWnd, _message, _wParam, _lParam);
    }
    return 0;
}

WindowInfo* WindowApp::GetWindowInfo()
{
    if (nullptr != windowInfo)
    {
        return windowInfo;
    }
    return nullptr;
}

void WindowApp::SetWindowSize(int _width ,int _height)
{
    windowInfo->screenWidth = _width;
    windowInfo->screenHeight = _height;

#ifdef IMGUIFLAG
    IMGUI->SetWindowSize(_width, _height);
#endif
}

void WindowApp::Initialize()
{
    RECT rcClient = { 0,0, windowInfo->screenWidth , windowInfo->screenHeight };
    AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

    // 메뉴바 포함한 크기 조정
    int adjustedWidth = rcClient.right - rcClient.left;
    int adjustedHeight = rcClient.bottom - rcClient.top;

    int midX = (GetSystemMetrics(SM_CXSCREEN) - adjustedWidth) / 2;
    int midY = (GetSystemMetrics(SM_CYSCREEN) - adjustedHeight) / 2;
    // WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME
    // WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MINIMIZEBOX;
    // WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX;
    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX;
      
    if(true == windowInfo->windoweMode)
    { // 창모드 일때 
        windowInfo->hWnd = CreateWindowEx(0, StringConverter::StringToWide(windowClassName).c_str(),
            StringConverter::StringToWide(gameName).c_str(), dwStyle,
            midX, midY, adjustedWidth, adjustedHeight, NULL, NULL, hInstance, NULL);
    }
    else
    {
        windowInfo->screenWidth = GetSystemMetrics(SM_CXSCREEN);
        windowInfo->screenHeight = GetSystemMetrics(SM_CYSCREEN);

        windowInfo->hWnd = CreateWindowEx(0, StringConverter::StringToWide(windowClassName).c_str(),
            StringConverter::StringToWide(gameName).c_str(),
            WS_EX_TOPMOST | WS_POPUP, 0, 0,
            GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
            NULL, NULL, hInstance, NULL);
    }

    if (!windowInfo->hWnd) { return; }
   
    SetWindowLongPtr(windowInfo->hWnd, GWL_STYLE, dwStyle);   // 창 크기 조정 비활성화: 창 스타일 변경
    ShowWindow(windowInfo->hWnd, SW_SHOW);
    UpdateWindow(windowInfo->hWnd);

    // 윈도우를 화면에 표시하고 포커스를 지정
    SetFocus(windowInfo->hWnd);
    SetForegroundWindow(windowInfo->hWnd);
    return;
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
