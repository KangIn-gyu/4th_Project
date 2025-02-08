// Engine.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
#include "pch.h"
#include "Engine.h"
#include "WindowApp.h"

#include "SystemHeader.h"
#include "Helper.h"
#include "SceneManager.h"
#include "Declare.h" 
#include "GameManager.h"

void Engine::Initialize()
{
    inputSystem = DXINPUT;
    graphicsSystem = RENDERER;
    timeSystem = TIMESYSTEM;
    sceneManager = SCENEMANAGER;

    if (nullptr != clientApp) // 윈도우 생성한게 있는가?
    {
        inputSystem->Initialize(clientApp->GetWindowInfo()->hWnd);
        RENDERER->Initialize(clientApp->GetWindowInfo());
        timeSystem->Initialize();
    }

    clientApp->Enter();
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
            // 추후 다양한 업데이트 만들자.
            float deltaTime = timeSystem->GetFloatDeltaTime();
 
            Update(deltaTime);
            RateUpdate(deltaTime);

            if (nullptr != clientGameManager)
            {
                clientGameManager->Update(deltaTime);
            }

            Render(deltaTime); // 시간이 과연 필요할가? 일단 보류
        }
    }

    if (msg.message == WM_NULL)
    {
        UnregisterClass(StringConverter::StringToWide(clientApp->GetWindowClassName()).c_str(), clientApp->GethInstance());  
    }
}

WindowInfo* Engine::GetWindowInfo() const
{
    if (nullptr != clientApp)
    {
        return clientApp->GetWindowInfo();
    }

    return nullptr;
}

void Engine::SetWindowSize(int _width, int _height)
{
    clientApp->SetWindowSize(_width, _height);
    graphicsSystem->SetWindowSize();
}

DXMath::Vector2 Engine::GetWindowSize()
{
    return DXMath::Vector2(clientApp->GetWindowInfo()->screenWidth, clientApp->GetWindowInfo()->screenHeight);
}

void Engine::CollectionGameManager(GameManager* _gameManager)
{
    clientGameManager = _gameManager;
}

Engine::~Engine()
{
}

void Engine::Update(const float _deltaTime)
{
    inputSystem->Update(_deltaTime);
    sceneManager->Update(_deltaTime);
    graphicsSystem->Update(_deltaTime);
}

void Engine::RateUpdate(const float _deltaTime)
{
    sceneManager->RateUpdate(_deltaTime);
}

void Engine::Render(const float _deltaTime)
{
    graphicsSystem->Render();
}



