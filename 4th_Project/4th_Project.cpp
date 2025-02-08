#include "pch.h"
#include "framework.h"
#include <windows.h> 

#include "Client.h"
#include "../Engine/Engine.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    Client* DemoApp = new Client(hInstance, "test", 1920, 1080, true);
    ENGINE->Initialize();
    ENGINE->Loop();

//    _CrtDumpMemoryLeaks();

    delete DemoApp;
}