#include "pch.h"
#include "WindowManager.h"

#include "Engine.h"
// extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WindowManager::WindowManager(HINSTANCE hInstance, std::wstring_view GameName, int ScreenWidth, int ScreenHeight, bool isFullScreen) : \
    m_hInstance(hInstance), m_strGamName(GameName), m_fScreenWidth(ScreenWidth), m_fScreenHeight(ScreenHeight), m_isFullScreen(isFullScreen)
{
    ENGINE->SetWindow(this);
    WindowsRegistration();
    Initialize(); 
}

WindowManager::~WindowManager()
{
}

LRESULT WindowManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
//        return true;
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_SETFOCUS:
        break;
    case WM_KILLFOCUS:
        break;
    case WM_ENTERSIZEMOVE:
        break;
    case WM_EXITSIZEMOVE:
        break;

    case WM_ACTIVATEAPP:
    //    DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
    //    DirectX::Mouse::ProcessMessage(message, wParam, lParam);
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
    //    DirectX::Mouse::ProcessMessage(message, wParam, lParam);
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
    //    DirectX::Keyboard::ProcessMessage(message, wParam, lParam);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

bool WindowManager::Initialize()
{
    RECT rcClient = { 0,0, m_fScreenWidth , m_fScreenHeight };
    AdjustWindowRect(&rcClient, WS_OVERLAPPEDWINDOW, FALSE);

    int midX = (GetSystemMetrics(SM_CXSCREEN) - m_fScreenWidth) / 2;
    int midY = (GetSystemMetrics(SM_CYSCREEN) - m_fScreenHeight) / 2;

    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME;

    if(true == m_isFullScreen)
    {
        m_fScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        m_fScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        m_hWnd = CreateWindowEx(0, L"4th_Project", m_strGamName.c_str(), WS_POPUP,
            0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, m_hInstance, NULL);
    }
    else
    {
        m_hWnd = CreateWindowEx(0, L"4th_Project", m_strGamName.c_str(), dwStyle,
            midX, midY, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
            NULL, NULL, m_hInstance, NULL);
    }

    if (!m_hWnd) { return FALSE; }
   
    SetWindowLongPtr(m_hWnd, GWL_STYLE, dwStyle);   // 창 크기 조정 비활성화: 창 스타일 변경
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    return TRUE;
}

ATOM WindowManager::WindowsRegistration()
{
    WNDCLASSEXW wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.hInstance = m_hInstance;
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
