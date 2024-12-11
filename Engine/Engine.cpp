// Engine.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
#include "pch.h"
#include "Engine.h"
#include "WindowApp.h"
#include "DirectXInput.h"
#include "Graphics.h"
#include "Helper.h"
void Engine::Initialize()
{
    inputSystem = DXINPUT;
    graphicsSystem = GRAPHICS;

    if (nullptr != clientApp)
    {
        inputSystem->Initialize(clientApp->GetWindowHandle());
    }
   
}

void Engine::Loop()
{
    MSG msg;

    ZeroMemory(&msg, sizeof(msg));

    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            //윈도우 메시지 처리 
            TranslateMessage(&msg); // 키입력관련 메시지 변환  WM_KEYDOWN -> WM_CHAR
            DispatchMessage(&msg);
        }
        else
        {
          
        }
    }

    if (msg.message == WM_NULL)
    {
        UnregisterClass(StringConverter::StringToWide(clientApp->GetWindowClassName()).c_str(), clientApp->GethInstance());  
    }
}

void Engine::Update(const float _deltaTime)
{
    inputSystem->Update(_deltaTime);
}

