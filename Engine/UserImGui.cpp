#include "pch.h"
#include "UserImGui.h"
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <ImGuizmo.h>

#include "WindowApp.h"
#include "D3DClass.h"
#include "Renderer.h"

#define TEXT_UTF8(text) reinterpret_cast<const char*>(text) // 한글 출력 매크로 문자열 옆에 접두어 u8해야됨

void UserImGui::Initialize(HWND _hwnd, ComPtr<ID3D11Device> _Device, ComPtr<ID3D11DeviceContext> _DeviceContext)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = &ImGui::GetIO(); (void)io;	// IO 구조(마우스/키보드/게임패드 입력, 시간, 다양한 구성 옵션/플래그)에 액세스
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard 
					| ImGuiConfigFlags_DockingEnable
					| ImGuiConfigFlags_ViewportsEnable; // 키보드 컨트롤 활성화  // ImGuiConfigFlags_ViewportsEnable
	io->Fonts->AddFontFromFileTTF("Resource/Font/DNFBitBitv2.ttf", 20.0f, NULL, io->Fonts->GetGlyphRangesKorean());

	// Dear ImGui 스타일 설정
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;  // 창 모서리 둥글게 만들지 않음
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;  // 배경 투명도 설정
	}
	io->DisplaySize = ImVec2(static_cast<float>(RENDERER->GetWindowsSize().first), static_cast<float>(RENDERER->GetWindowsSize().second));
	// 플랫폼 / 렌더러 백엔드 설정
	ImGui_ImplWin32_Init(_hwnd);
	ImGui_ImplDX11_Init(_Device.Get(), _DeviceContext.Get());
}

void UserImGui::Update(const float _deltaTime)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGuizmo::BeginFrame();
	ImGuizmo::SetOrthographic(false);  // 투시 뷰 사용 여부
//	ImGuizmo::SetDrawlist();
}

void UserImGui::Render()
{
//	ImGui::ShowDemoWindow(); 데모
	MainDockSpace();

	// 렌더링
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	// 멀티 뷰포트 렌더링 처리
	if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
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
				is_mainMenu = true;
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
	if (true == is_consoleMenu)
	{
		ImGui::Begin(TEXT_UTF8(u8"콘솔"), &is_consoleMenu, ImGuiWindowFlags_AlwaysAutoResize); // ImGuiWindowFlags_NoMove
		
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

void UserImGui::MainMenu()
{
	if (true == is_mainMenu)
	{
		ImGui::Begin(TEXT_UTF8(u8"메인 도구"), &is_mainMenu, ImGuiWindowFlags_AlwaysAutoResize);
		static float color[4] = { backgroundColor.R(), backgroundColor.G(), backgroundColor.B(), backgroundColor.A()};

		// ImGui ColorEdit4 사용
		if (ImGui::ColorEdit4("clear color", color))
		{
			// 변경된 색상 값을 backgroundColor에 다시 할당
			backgroundColor = DXMath::Color(color[0], color[1], color[2], color[3]);
		}
		ImGui::End();
	}
}

void UserImGui::MainDockSpace()
{
	// 도킹 공간 처리
//  TODO : 유니티처럼 만들려고 했으나 너무 복잡해서 포기
	ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");
//	ImGui::DockSpaceOverViewport(ImGui::GetWindowDockID());

	MenuBar();
	MainMenu();
	ConsoleMenu();
	
// TODO : ImGui 보류 사항 도킹 스페이스를 하나 더 만들어야 할 상황이 있을가?
	if (io->ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
	//	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
		if (ImGui::Begin("MyDockedWindow"))

		ImGui::End();
	}
}

UserImGui::~UserImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

