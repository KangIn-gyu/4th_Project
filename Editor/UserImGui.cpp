#include "pch.h"
#include "UserImGui.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

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
}

void UserImGui::Update(const float _deltaTime)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void UserImGui::Render()
{
//	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

UserImGui::~UserImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

