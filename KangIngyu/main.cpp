#include "pch.h"
#include <windows.h> 

#include "ClientApp.h"
#include "../Engine/Engine.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    #ifdef _DEBUG
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    ClientApp* DemoApp = new ClientApp(hInstance, "test", 1024, 768, true);

 //   _CrtSetBreakAlloc(1025388);

    ENGINE->Initialize();
    ENGINE->Loop();
    //    _CrtDumpMemoryLeaks();

    delete DemoApp;
}