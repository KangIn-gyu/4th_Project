// Engine.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
#include "pch.h"
#include "Engine.h"
#include "WindowApp.h"

#include "SystemHeader.h"
#include "Helper.h"
#include "Declare.h" // 윈도우 정보때문에

// test
#include "ResourceSystem.h"

void Engine::Initialize()
{
    inputSystem = DXINPUT;
    graphicsSystem = RENDERER;
    timeSystem = TIMESYSTEM;
    if (nullptr != clientApp)
    {
        inputSystem->Initialize(clientApp->GetWindowInfo()->hWnd);
        RENDERER->Initialize(clientApp->GetWindowInfo());
        timeSystem->Initialize();
    }
    std::shared_ptr<Shader> test = RESOURCESYSTEM->Load<Shader>(L"../Engine/VertexShaderVS.hlsl");
//    std::shared_ptr<Shader> test1 = RESOURCESYSTEM->Load<Shader>(L"../PixelShaderPS.hlsl");
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
            timeSystem->Update();
            Update(timeSystem->GetFloatDeltaTime());
            Render(timeSystem->GetFloatDeltaTime());
        }
    }

    if (msg.message == WM_NULL)
    {
        UnregisterClass(StringConverter::StringToWide(clientApp->GetWindowClassName()).c_str(), clientApp->GethInstance());  
    }
}

Engine::~Engine()
{
}

void Engine::Update(const float _deltaTime)
{
    inputSystem->Update(_deltaTime);
}

void Engine::Render(const float _deltaTime)
{
    RENDERER->Render();
}



