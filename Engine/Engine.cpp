// Engine.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
#include "pch.h"
#include "Engine.h"
#include "WindowApp.h"

#include "SystemHeader.h"
#include "Helper.h"
#include "Declare.h" // 윈도우 정보때문에

#include "Texture.h"
#include "Shader.h"
#include "ResourceSystem.h"
void Engine::TestCode()
{
   std::shared_ptr<Texture> test = RESOURCESYSTEM->Load<Texture>(L"STAGE1/Texturs/dice.png");
   std::shared_ptr<Texture> test1 = RESOURCESYSTEM->Load<Texture>(L"STAGE1/Texturs/Base_BaseColor.tga");
   std::shared_ptr<Texture> test3 = RESOURCESYSTEM->Load<Texture>(L"STAGE1/Texturs/SkyBlueBrdf.dds");

   std::shared_ptr<Shader> sh2 = RESOURCESYSTEM->Load<Shader>(L"STAGE1/Shaders/VertexShaderVS.hlsl");
   std::shared_ptr<Shader> sh1 = RESOURCESYSTEM->Load<Shader>(L"STAGE1/Shaders/PixelShaderPS.hlsl");
   RESOURCESYSTEM->Show();
}


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

    TestCode();
}

void Engine::Loop()
{
    MSG msg;

    ZeroMemory(&msg, sizeof(msg));
    timeSystem->Update();

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
            // 추후 다양한 업데이트 만들자.
            float deltaTime = timeSystem->GetFloatDeltaTime();
            Update(deltaTime);
            Render(deltaTime); // 시간이 과연 필요할가? 일단 보류
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



