#include "pch.h"
#include "UserImGui.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include "WindowApp.h"

#define TEXT_UTF8(text) reinterpret_cast<const char*>(text) // 한글 출력 매크로 문자열 옆에 접두어 u8해야됨

void UserImGui::Initialize(HWND _hwnd, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _DeviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Dear ImGui 스타일 설정
	ImGui::StyleColorsDark();

	// 플랫폼 / 렌더러 백엔드 설정
	ImGui_ImplWin32_Init(_hwnd);
	ImGui_ImplDX11_Init(_Device.Get(), _DeviceContext.Get()); // 고민 사항

	io = &ImGui::GetIO(); (void)io;	// IO 구조(마우스/키보드/게임패드 입력, 시간, 다양한 구성 옵션/플래그)에 액세스
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 키보드 컨트롤 활성화
	io->Fonts->AddFontFromFileTTF("Resource/Font/DNFBitBitv2.ttf", 20.0f, NULL, io->Fonts->GetGlyphRangesKorean());
}

void UserImGui::Update(const float _deltaTime)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void UserImGui::Render()
{
//	ImGui::ShowDemoWindow(); 데모
	MenuBar();
	ConsoleMenu();

	{ 
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
}

void UserImGui::MenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu(TEXT_UTF8(u8"도구")))
		{
			if (ImGui::MenuItem(TEXT_UTF8(u8"메뉴")))
			{
				
			}
			if (ImGui::MenuItem(TEXT_UTF8(u8"콘솔창")))
			{
				is_consoleMenu = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void UserImGui::ConsoleMenu()
{
	if (is_consoleMenu)
	{
		ImGui::Begin(TEXT_UTF8(u8"콘솔"), &is_consoleMenu, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
		
		// 콘솔의 표시/숨기기
		static bool consoleVisible = true;
		if (ImGui::Button(consoleVisible ? TEXT_UTF8(u8"숨김") : TEXT_UTF8(u8"콘솔 보기")))
		{
			if (consoleVisible)
			{
				WindowApp::console->HideConsole();
			}
			else
			{
				WindowApp::console->ShowConsole();
			}
			consoleVisible = !consoleVisible;
		}

		// 글꼴 크기 조정
		static int fontWidth = 10, fontHeight = 15;
		ImGui::InputInt("Font Width", &fontWidth);
		ImGui::InputInt("Font Height", &fontHeight);
		if (ImGui::Button("Set Font Size"))
		{
			WindowApp::console->SetFontSize(fontWidth, fontHeight);
		}

		ImGui::Dummy(ImVec2(0.0f, 10.0f)); // 여백 추가

		// 콘솔 크기 변경
		static int consoleWidth = 70, consoleHeight = 60;
		ImGui::InputInt("Console Width", &consoleWidth);
		ImGui::InputInt("Console Height", &consoleHeight);
		if (ImGui::Button("Resize Console"))
		{
			WindowApp::console->ResizeConsole(consoleWidth, consoleHeight);
		}
		ImGui::End();
	}
}

UserImGui::~UserImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

